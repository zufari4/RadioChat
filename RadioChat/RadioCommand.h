#pragma once

#include <stdint.h>

enum class RadioCommand: uint8_t
{
    MessageNew,
    MessageDelivered,
    Ping,
    PingDelivered
};