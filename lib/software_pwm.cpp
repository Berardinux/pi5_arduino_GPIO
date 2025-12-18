#include "internal_pwm_state.h"
#include <unistd.h>

void software_pwm_worker(int pin) {
    const int period_us = 1000; // 1 kHz PWM

    while (pwm_running[pin]) {
        int value = pwm_value[pin];

        if (value <= 0) {
            gpiod_line_request_set_value(
                pin_request[pin],
                pin,
                GPIOD_LINE_VALUE_INACTIVE
            );
            usleep(period_us);
            continue;
        }

        if (value >= 255) {
            gpiod_line_request_set_value(
                pin_request[pin],
                pin,
                GPIOD_LINE_VALUE_ACTIVE
            );
            usleep(period_us);
            continue;
        }

        int on_time  = (period_us * value) / 255;
        int off_time = period_us - on_time;

        gpiod_line_request_set_value(
            pin_request[pin],
            pin,
            GPIOD_LINE_VALUE_ACTIVE
        );
        usleep(on_time);

        gpiod_line_request_set_value(
            pin_request[pin],
            pin,
            GPIOD_LINE_VALUE_INACTIVE
        );
        usleep(off_time);
    }
}
