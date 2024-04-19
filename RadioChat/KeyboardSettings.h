#pragma once

#include <stdint.h>
#include "Language.h"

struct KeyboardSettings // SN74HC165N
{
    struct Pins 
    {
        uint8_t SH_LD = 5; // pin 1 
        uint8_t INH   = 17;  // pin 15
        uint8_t QH    = 16;  // pin 9 
        uint8_t CLK   = 4;  // pin 2 
    } pins;
    Language lang = Language::Russian;
    uint8_t  maxKeyNum = 40;
    uint8_t  fnKey = 2;
    uint8_t  enterKey = 38;
    uint8_t  countRegisters = 5;
};