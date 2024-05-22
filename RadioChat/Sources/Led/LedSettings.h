#pragma once

#include <stdint.h>

struct LedSettings
{
    struct Pins {
        uint8_t on = 5;
    } pins;
    uint16_t interval;
};