#pragma once

#include <stdint.h>

struct FlashSettings
{
    struct Pins {
        uint8_t sd0 = 16;
    } pins;
};