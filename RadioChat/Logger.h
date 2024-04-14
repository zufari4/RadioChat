#pragma once

#include "Configuration.h"

#if DEBUG_MODE == 1

#include <HardwareSerial.h>
#define INIT_LOG() Serial.begin(SERIAL_BAUDRATE)
#define LOG(...) Serial.printf(__VA_ARGS__)

#else

#define INIT_LOG()
#define LOG(...)

#endif