#include <gpiod.h>
#include <unistd.h>

class GPIO {
    struct gpiod_chip *chip;
    struct gpiod_line_request *req;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *config;
    unsigned int line;

    public:
        GPIO(unsigned int gpio_line) : line(gpio_line){
            chip = gpiod_chip_open("/dev/gpiochip0");
    
            settings = gpiod_line_settings_new();
            gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    
            config = gpiod_line_config_new();
            gpiod_line_config_add_line_settings(config, &line, 1, settings);
    
            req = gpiod_chip_request_lines(chip, nullptr, config);
        }
    
        void on() {
            gpiod_line_request_set_value(req, line, GPIOD_LINE_VALUE_ACTIVE);
        }
    
        void off() {
            gpiod_line_request_set_value(req, line, GPIOD_LINE_VALUE_INACTIVE);
        }
};

int main() {
    GPIO gpio14(14);
    GPIO gpio15(15);
    GPIO gpio18(18);

    while (1) {
        gpio14.on();
        usleep(500000);

        gpio15.on();
        usleep(500000);

        gpio18.on();
        usleep(500000);

        gpio14.off();
        usleep(500000);

        gpio15.off();
        usleep(500000);

        gpio18.off();
        usleep(500000);
    }
}

