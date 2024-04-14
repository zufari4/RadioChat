#pragma once

#include "PinSet.h"

struct DisplaySettings
{
    PinSet::Display pins;
    uint16_t brightnessLevel = 1024;
};