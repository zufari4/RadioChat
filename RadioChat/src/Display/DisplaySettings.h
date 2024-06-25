#pragma once

#include "../Configuration.h"
#include <stdint.h>

struct DisplaySettings // LCD 12864
{
    struct Pins 
    {
        uint8_t RS  = 2;  // SPI SS, CS, Chip Select
        uint8_t R_W = 15; // SPI MOSI, data
        uint8_t E   = 12; // SPI SCK, CLK, clock 
        uint8_t RST = 255;// Reset U8X8_PIN_NONE
        uint8_t BLA = 17; // BLA, brigthnes

    } pins;
    uint16_t brightnessLevel = 255;
};