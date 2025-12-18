#ifndef ARDUINO_GPIO_H
#define ARDUINO_GPIO_H
#include <string>

enum PinMode {INPUT, OUTPUT};
enum PinValue {LOW, HIGH};

enum PwmMode {
    PWM_NONE,
    PWM_SOFTWARE,
    PWM_HARDWARE_CH0,
    PWM_HARDWARE_CH1
};

void pinMode(int pin, PinMode mode);
void digitalWrite(int pin, PinValue value);
PinValue digitalRead(int pin);
void analogWrite(int pin, int value);
void delay(unsigned int ms);
void delayMicroseconds(unsigned int us);
void print(const std::string& s);
void println(const std::string& s);
unsigned long millis();
unsigned long micros();

#endif
