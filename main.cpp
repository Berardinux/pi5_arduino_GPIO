#include "lib/arduino_gpio.h"
#include <unistd.h>

/**
 *  _________       _________    _________                              _________      ______          _______________    __            _    _          _     _         _    ___
 * |  ______ \     |  _______|  |  ______ \              /\            |  ______ \    |  ___ \        |______   ______|  |   \         | |  | |        | |   \ \       / /  |   | 
 * | |      \ \    | |          | |      \ \            /  \           | |      \ \   | |   \ \              | |         | |\ \        | |  | |        | |    \ \     / /   |   |
 * | |       \ \   | |          | |       \ \          / /\ \          | |       \ \  | |    \ \             | |         | | \ \       | |  | |        | |     \ \   / /    |   |
 * | |       / /   | |          | |       / /         / /  \ \         | |       / /  | |     \ \            | |         | |  \ \      | |  | |        | |      \ \_/ /     |   |
 * | |______/ /    | |_______   | |______/ /         / /____\ \        | |______/ /   | |      \ \           | |         | |   \ \     | |  | |        | |       \   /      |   |
 * | |______ |     |  _______|  |  ___   _/         / _______  \       |  ___   _/    | |       | |          | |         | |    \ \    | |  | |        | |        | |       |   |
 * | |      \ \    | |          | |   \ \          / /        \ \      | |   \ \      | |      / /           | |         | |     \ \   | |  | |        | |       / _ \      |   |
 * | |       \ \   | |          | |    \ \        / /          \ \     | |    \ \     | |     / /            | |         | |      \ \  | |  | |        | |      / / \ \     |___|
 * | |       / /   | |          | |     \ \      / /            \ \    | |     \ \    | |    / /             | |         | |       \ \ | |  | |        | |     / /   \ \     ___
 * | |______/ /    | |_______   | |      \ \    / /              \ \   | |      \ \   | |___/ /        ______| |______   | |        \ \| |  \  \______/  /    / /     \ \   |   |
 * |_________/     |_________|  |_|       \_\  /_/                \_\  |_|       \_\  |______/        |_______________|  |_|         \ __|   \__________/_\  /_/       \_\  |___|
 */

int main() {
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(20, OUTPUT);
    pinMode(23, OUTPUT);

    pinMode(21, INPUT);

    while (1) {
        auto varSwitchState = digitalRead(21);

        if (varSwitchState == LOW) {
            /**
             * With this library, pin 12 and 13 are set up to be used as analogWrite() only.
             * They use hardware PWM for more precise motor control. You do NOT need to 
             * initialize pin 12 and 13 to use them, but if you do it will not hurt anything, 
             * it will just give you a warning.
             */
            analogWrite(12, 128);
            analogWrite(13, 64);
            /**
             * You can do analogWrite() on other pins besides 12 and 13, it will just be a software 
             * PWM signal. You will still need to initialize all pins other than pin 12 and 13 to 
             * use them.
             */

            unsigned long time_ms = millis();
            print("Program uptime: ");
            print(std::to_string(time_ms));
            println("ms");

            unsigned long time_us = micros();
            println("The program has ran for: " + std::to_string(time_us) + "us");

            digitalWrite(14, HIGH);
            delay(150);
    
            digitalWrite(15, HIGH);
            delay(150);
    
            digitalWrite(18, HIGH);
            delay(150);

            digitalWrite(23, HIGH);
            delay(150);
    
            digitalWrite(14, LOW);
            delayMicroseconds(150000);
    
            digitalWrite(15, LOW);
            delay(150);
    
            digitalWrite(18, LOW);
            delay(150);

            digitalWrite(23, LOW);
            delay(150);
        } else {
            analogWrite(12, 192);
            analogWrite(13, 128);

            digitalWrite(14, HIGH);
            digitalWrite(15, HIGH);
            digitalWrite(18, HIGH);
            digitalWrite(23, HIGH);
            delay(150);
            digitalWrite(14, LOW);
            digitalWrite(15, LOW);
            digitalWrite(18, LOW);
            digitalWrite(23, LOW);
            delay(150);
        }
    }
}

