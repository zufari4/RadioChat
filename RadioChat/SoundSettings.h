#pragma once

#include <stdint.h>

struct SoundSettings
{
    struct Pins
    {
        uint8_t io = 27;
    } pins;
    uint8_t tempo = 108;
};
