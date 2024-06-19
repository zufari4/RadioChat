#pragma once

#include <stdint.h>

struct SoundSettings
{
    struct Pins
    {
        uint8_t io = 22;
    } pins;
    uint8_t tempo = 108;
};
