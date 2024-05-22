#pragma once

#include <stdint.h>

struct DisplaySettings // LCD 12864
{
    struct Pins 
    {
        uint8_t RS  = 15; //Display[RS] -> ESP8266[IO15, CS, Chip Select]
        uint8_t led = 2; //Display[BLA] -> ESP8266[IO2, 0.0-5.0 V]
    } pins;
    uint16_t brightnessLevel = 1024;
};