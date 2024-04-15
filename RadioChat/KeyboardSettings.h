#include <stdint.h>
#pragma once

#include "PinSet.h"
#include "Language.h"

struct KeyboardSettings
{
    Language lang = Language::Russian;
    PinSet::Keyboard pins;
    uint8_t maxKeyNum = 40;
    uint8_t fnKey = 2;
    uint8_t enterKey = 38;
    uint8_t countRegisters = 5;
};