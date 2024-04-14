#pragma once

#include "Configuration.h"

#if DEBUG_MODE == 1

#include "Utils.h"
#include <HardwareSerial.h>
#define INIT_LOG() { Serial.begin(SERIAL_BAUDRATE); Serial.printf("Init log\n"); Serial.flush(); }
#define LOG(...) { Serial.printf("%s: ", utils::currentDateTime()); Serial.printf(__VA_ARGS__); }

#else

#define INIT_LOG()
#define LOG(...)

#endif