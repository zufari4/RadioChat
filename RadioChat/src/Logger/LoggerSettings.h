#pragma once

#include <stdint.h>
#include <string>
#include "LogTraceLevel.h"
#include "../Configuration.h"

struct LoggerSettings
{
#if DEBUG_MODE == 1
    LogTraceLevel level = LogTraceLevel::Debug;
    bool logToSerial = true;
#else
    LogTraceLevel level = LogTraceLevel::Error;
    bool logToSerial = false;
#endif
    std::string logPath = "logs";
    uint16_t maxCountLogs  = 10;
    uint16_t maxCountLines = 2000;
    uint16_t maxMessageSize = 256;
};