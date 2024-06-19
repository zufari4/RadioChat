#pragma once

#include <stdint.h>

struct LedSettings
{
    struct Pins {
        uint8_t on = 21;
    } pins;
    uint16_t interval = 1000;
};