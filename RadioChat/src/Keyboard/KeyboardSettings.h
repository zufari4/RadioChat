#pragma once

#include <stdint.h>
#include "Language.h"

struct KeyboardSettings // SN74HC165N
{
    struct Pins 
    {
        uint8_t SH_LD = 16;  // OUTPUT pin 1 on SN74HC165N
        uint8_t QH    = 13;  // INPUT  pin 9 on SN74HC165N
        uint8_t CLK   = 14;  // OUTPUT pin 2 on SN74HC165N
    } pins;
    Language lang = Language::Russian;
    uint8_t  maxKeyNum = 40;
    uint8_t  fnKey = 2;
    uint8_t  enterKey = 38;
    uint8_t  countRegisters = 5;
};