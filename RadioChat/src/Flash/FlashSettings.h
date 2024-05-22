#pragma once

#include <stdint.h>
#include <string>

struct FlashSettings
{
    struct Pins {
        uint8_t miso = 19;
        uint8_t mosi = 23;
        uint8_t sck = 18;
        uint8_t cs = 5;
    } pins;
    bool useHWspi = true;
};