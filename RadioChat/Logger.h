#pragma once

#include "Configuration.h"

#if DEBUG_MODE == 1

#include "Utils.h"
#include <HardwareSerial.h>
#define INIT_LOG() { Serial.begin(SERIAL_BAUDRATE); delay(5000); Serial.printf("\nInit log\n"); }
#define LOG(...) { Serial.printf("%s: ", utils::currentDateTime()); Serial.printf(__VA_ARGS__); }
#define LOGX(...) { Serial.printf(__VA_ARGS__); }

#else

#define INIT_LOG()
#define LOG(...)
#define LOGX(...)

#endif