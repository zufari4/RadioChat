#pragma once

#include <stdint.h>

enum class LogTraceLevel: uint8_t
{
    None,
    Error,
    Info,
    Debug
};