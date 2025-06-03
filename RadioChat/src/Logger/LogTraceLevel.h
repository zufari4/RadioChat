#pragma once

#include <string_view>

enum class LogTraceLevel: uint8_t
{
    None,
    Error,
    Info,
    Debug
};

std::string_view traceLvlToStr(LogTraceLevel level);
std::string_view traceLvlToLog(LogTraceLevel level);