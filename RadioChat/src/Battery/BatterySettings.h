#pragma once

#include <stdint.h>

struct BatterySettings
{
    uint8_t  pinVotage = 4;
    float    maxBatteryVoltage = 8.4f;
    float    maxADC  = 4095.0f;
    float    cFactor = 0.6f;
    uint16_t checkInterval = 1000;
};