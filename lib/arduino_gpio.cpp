#include "arduino_gpio.h"
#include "software_pwm.h"
#include "hardware_pwm.h"
#include "internal_pwm_state.h"
#include <gpiod.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <string>
#include <chrono>

struct gpiod_chip *chip = nullptr;
struct gpiod_line_settings *settings = nullptr;
struct gpiod_line_config *config = nullptr;

static std::thread pwm_thread[64];
struct gpiod_line_request *pin_request[64] = { nullptr };
std::atomic<int> pwm_value[64];
std::atomic<bool> pwm_running[64];

// ------------------------------------------------------------
// <Set up the pins>
// ------------------------------------------------------------
void pinMode(int pin, PinMode mode) {
    if (pin == 12 || pin == 13) {
        std::cout << "WARNING:: Pins 12 and 13 do not need to be initialized, they are used for analogWrite only.\n";
        return;
    }

    chip = gpiod_chip_open("/dev/gpiochip0");
    settings = gpiod_line_settings_new();

    switch (mode)
    {
        case OUTPUT:
            gpiod_line_settings_set_direction(
                settings,
                GPIOD_LINE_DIRECTION_OUTPUT
            );
            break;

        case INPUT:
            gpiod_line_settings_set_direction(
                settings,
                GPIOD_LINE_DIRECTION_INPUT
            );
            break;
    }

    config = gpiod_line_config_new();
    unsigned int line = (unsigned int)pin;

    gpiod_line_config_add_line_settings(
        config,
        &line,
        1,
        settings
    );

    pin_request[pin] = gpiod_chip_request_lines(
        chip,
        nullptr,
        config
    );
}
// ------------------------------------------------------------
// </Set up the pins>
// ------------------------------------------------------------

// ------------------------------------------------------------
// <Digital Write>
// ------------------------------------------------------------
void digitalWrite(int pin, PinValue value) {
    if (pin == 12 || pin == 13) {
        std::cout << "ERROR:: Pin" << pin << " can only be used with analogWrite(" << pin << ", ***);\n";
        return;
    }

    auto req = pin_request[pin];
    if (!req) return;

    switch (value)
    {
        case HIGH:
            gpiod_line_request_set_value(
                req,
                pin,
                GPIOD_LINE_VALUE_ACTIVE
            );
            break;

        case LOW:
            gpiod_line_request_set_value(
                req,
                pin,
                GPIOD_LINE_VALUE_INACTIVE
            );
            break;
    }
}

// ------------------------------------------------------------
// </Digital Write>
// ------------------------------------------------------------

// ------------------------------------------------------------
// <Digital Read>
// ------------------------------------------------------------
PinValue digitalRead(int pin) {
    if (pin == 12 || pin == 13) {
        std::cout << "ERROR:: Pin" << pin << " can only be used with analogWrite(" << pin << ", ***);\n";
        return LOW;
    }

    auto req = pin_request[pin];
    if (!req) return LOW;

    int value = gpiod_line_request_get_value(req, pin);

    if (value > 0)
    {
        return HIGH;
    }
    else
        return LOW;
}
// ------------------------------------------------------------
// </Digital Read>
// ------------------------------------------------------------

// ------------------------------------------------------------
// <Analog Write>
// ------------------------------------------------------------
void analogWrite(int pin, int value) {
    auto req = pin_request[pin];
    if (!req && !(pin == 12 || pin == 13)) {
        return;
    }

    if (value < 0)   value = 0;
    if (value > 255) value = 255;

    pwm_value[pin] = value;

    if (pin == 12) {
        int ch = 0;
        hardware_pwm_set_duty(ch, value);
        return;
    } else if (pin == 13) {
        int ch = 1;
        hardware_pwm_set_duty(ch, value);
        return;
    } else {
        if (!pwm_running[pin])
        {
            pwm_running[pin] = true;
            pwm_thread[pin] = std::thread(software_pwm_worker, pin);
            pwm_thread[pin].detach();
        }
    }
}
// ------------------------------------------------------------
// </Analog Write>
// ------------------------------------------------------------

// ------------------------------------------------------------
// <Other>
// ------------------------------------------------------------

void delay(unsigned int ms) {
    usleep(ms * 1000);
}

void delayMicroseconds(unsigned int us) {
    usleep(us);
}

void print(const std::string& s) {
    std::cout << s;
}

void println(const std::string& s) {
    std::cout << s << "\n";
}

// Millis stuff

unsigned long millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}

// Micros stuff

unsigned long micros() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
}

// ------------------------------------------------------------
// </Other>
// ------------------------------------------------------------
