#include "hardware_pwm.h"
#include <fstream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>

static bool pwm_initialized[2] = { false, false };

static void write_sysfs(const std::string& path, int value) {
    std::ofstream f(path);
    if (f) {
        f << value;
    }
}

static void hardware_pwm_disable(int ch) {
    write_sysfs("/sys/class/pwm/pwmchip0/pwm" + std::to_string(ch) + "/duty_cycle", 0);
    write_sysfs("/sys/class/pwm/pwmchip0/pwm" + std::to_string(ch) + "/enable", 0);
}

static void pwm_cleanup() {
    for (int ch = 0; ch < 2; ch++) {
        if (pwm_initialized[ch]) {
            hardware_pwm_disable(ch);
        }
    }
}

static void pwm_signal_handler(int) {
    pwm_cleanup();
    _exit(0);
}

__attribute__((constructor))
static void pwm_setup() {
    atexit(pwm_cleanup);
    signal(SIGINT, pwm_signal_handler);
    signal(SIGTERM, pwm_signal_handler);
}

static void hardware_pwm_init(int ch) {
    if (pwm_initialized[ch]) {
        return;
    }

    write_sysfs("/sys/class/pwm/pwmchip0/export", ch);
    usleep(10000);

    write_sysfs("/sys/class/pwm/pwmchip0/pwm" + std::to_string(ch) + "/period", 1000000);
    write_sysfs("/sys/class/pwm/pwmchip0/pwm" + std::to_string(ch) + "/enable", 1);


    pwm_initialized[ch] = true;
}

void hardware_pwm_set_duty(int channel, int value) {
    if (channel < 0 || channel > 1) {
        return;
    }

    if (value < 0) {
        value = 0;
    }

    if (value > 255) {
        value = 255;
    }

    hardware_pwm_init(channel);

    int duty_ns = 1000000 * value / 255;

    write_sysfs("/sys/class/pwm/pwmchip0/pwm" + std::to_string(channel) + "/duty_cycle", duty_ns);
}
