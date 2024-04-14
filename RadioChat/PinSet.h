#pragma once

#include <stdint.h>

namespace PinSet {
    struct Keyboard
    {
        uint8_t SH_LD = 16; // pin 1 
        uint8_t INH   = 5;  // pin 15
        uint8_t QH    = 4;  // pin 9 
        uint8_t CLK   = 0;  // pin 2 
    };
    struct Display // LCD 12864
    {
        uint8_t RS = 15; //Display[RS] -> ESP8266[IO15, CS, Chip Select]
        uint8_t led = 2; //Display[BLA] -> ESP8266[IO2, 0.0-5.0 V]
    };
    struct Radio // Lora E22 400T33D
    {

    };
}
