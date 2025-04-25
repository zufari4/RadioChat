#pragma once

#include <stdint.h>

struct BatterySettings
{
    uint8_t  pinVotage = 35;
    float    maxBatteryVoltage = 8.4f;
    float    maxADC  = 4095.0f;
    float    cFactor = -0.4f;
    uint16_t checkInterval = 1000;
};