#include <gpiod.h>
#include <unistd.h>

enum PinMode {INPUT, OUTPUT};
enum PinValue {LOW, HIGH};

struct gpiod_chip *chip = nullptr;
struct gpiod_line_settings *settings = nullptr;
struct gpiod_line_config *config = nullptr;

static struct gpiod_line_request *pin_request[64] = {nullptr};


void pinMode(int pin, PinMode mode) {
    chip = gpiod_chip_open("/dev/gpiochip0");
    settings = gpiod_line_settings_new();

    switch (mode) {
        case OUTPUT:
            gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
            break;
        case INPUT:
            gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
            break;
    }

    config = gpiod_line_config_new();
    unsigned int line = (unsigned int)pin;
    gpiod_line_config_add_line_settings(config, &line, 1, settings);
    pin_request[pin] = gpiod_chip_request_lines(chip, nullptr, config);

}

void digitalWrite(int pin, PinValue value) {
    auto req = pin_request[pin];
    if (!req) return;

    switch (value) {
        case HIGH:
            gpiod_line_request_set_value(req, pin, GPIOD_LINE_VALUE_ACTIVE);
            break;
        case LOW:
            gpiod_line_request_set_value(req, pin, GPIOD_LINE_VALUE_INACTIVE);
            break;
    }
}

int main() {
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(18, OUTPUT);

    while (1) {
        digitalWrite(14, HIGH);
        usleep(500000);

        digitalWrite(15, HIGH);
        usleep(500000);

        digitalWrite(18, HIGH);
        usleep(500000);

        digitalWrite(14, LOW);
        usleep(500000);

        digitalWrite(15, LOW);
        usleep(500000);

        digitalWrite(18, LOW);
        usleep(500000);
    }
}

