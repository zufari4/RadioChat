#pragma once

#include <stdint.h>
#include "LogTraceLevel.h"
#include "Configuration.h"

struct LoggerSettings
{
#if DEBUG_MODE == 1
    LogTraceLevel level = LogTraceLevel::Debug;
    bool logToSerial = true;
#else
    LogTraceLevel level = LogTraceLevel::Error;
    bool logToSerial = false;
#endif
    char logPath[5] = {'l','o','g','s','\0'};
    uint8_t  maxCountLogs  = 10;
    uint8_t  maxCountLines = 2000;
    uint16_t serialBuadrate = 9600;
    uint16_t maxMessageSize = 256;
};