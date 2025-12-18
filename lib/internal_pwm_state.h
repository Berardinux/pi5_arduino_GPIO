#pragma once
#include <gpiod.h>
#include <atomic>

// Shared PWM state (single owner)
extern struct gpiod_line_request *pin_request[64];
extern std::atomic<int> pwm_value[64];
extern std::atomic<bool> pwm_running[64];

