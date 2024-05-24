#pragma once

#include <stdint.h>
#include <string>
#include "../Configuration.h"

struct FlashSettings
{
    struct Pins {
        uint8_t miso = 19;
        uint8_t mosi = 23;
        uint8_t sck = 18;
        uint8_t cs = FLASH_CS_PIN;
    } pins;
};