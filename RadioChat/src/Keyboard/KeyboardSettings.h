#pragma once

#include <stdint.h>
#include "Language.h"

struct KeyboardSettings // SN74HC165N
{
    struct Pins 
    {
        uint8_t SH_LD = 16; // OUTPUT
        uint8_t INH   = 4;  // OUTPUT
        uint8_t QH    = 0;  // INPUT
        uint8_t CLK   = 2;  // OUTPUT
    } pins;
    Language lang = Language::Russian;
    uint8_t  maxKeyNum = 40;
    uint8_t  fnKey = 2;
    uint8_t  enterKey = 38;
    uint8_t  countRegisters = 5;
};