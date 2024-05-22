#include "Logger.h"
#include "../Utils.h"
#include "../Configuration.h"
#include <HardwareSerial.h>
#include <stdarg.h>

Logger::Logger()
    : flash_(nullptr)
    , isInit_(false)
    , serialIsInit_(false)
    , buffer_(LOGGER_DEF_MSG_SIZE, '\0')
{
}

Logger::~Logger()
{
}

Logger& Logger::instance()
{
    static Logger singleton;
    return singleton;
}

void Logger::initSerialLogging()
{
    if (!serialIsInit_) {
        Serial.begin(SERIAL_SPEED);
        delay(SERIAL_INIT_DELAY); 
        Serial.printf("\nSerial initialized\n");
        serialIsInit_ = true;
    }
}

void Logger::init(const LoggerSettings& settings, Flash* flash)
{
    settings_ = settings;
    flash_ = flash;
    if (settings_.level == LogTraceLevel::None) 
        return; // no need log

    buffer_.resize(settings_.maxMessageSize + 25); // + date time
    if (settings_.logToSerial) initSerialLogging();
    isInit_ = true;
}

void Logger::log(LogTraceLevel level, const char* format, ...)
{
    if (!isInit_ && !serialIsInit_) 
        return;
    if ((uint8_t)level > (uint8_t)settings_.level) 
        return;

    char* dst = buffer_.data();
    uint8_t offset = utils::datetime_str(dst, buffer_.size());
    dst[offset++] = ' ';

    va_list arglist;
    va_start(arglist, format);
    vsnprintf(dst + offset, buffer_.size() - offset, format, arglist);
    va_end(arglist);

    if (settings_.logToSerial) 
        Serial.printf("%s\n", dst);
    if (flash_ != nullptr) {
        // TODO
    }
}

LogTraceLevel Logger::getLogLevel() const
{
    return settings_.level;
}