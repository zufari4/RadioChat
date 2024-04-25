#include "Logger.h"
#include "Utils.h"
#include <HardwareSerial.h>
#include <stdarg.h>

Logger::Logger()
    : flash_(nullptr)
{
}

Logger::~Logger()
{
}

void Logger::init(const LoggerSettings& settings, Flash* flash)
{
    settings_ = settings;
    flash_ = flash;

    if (settings_.level != LogTraceLevel::None) {
        if (settings_.logToSerial) {
            Serial.begin(settings_.serialBuadrate);
            delay(4000); 
            Serial.printf("\nSerial initialized");
        }
        buffer_.resize(settings_.maxMessageSize + 25); // + date time
    }
}

Logger& Logger::instance()
{
    static Logger singleton;
    return singleton;
}

void Logger::log(LogTraceLevel level, const char* format, ...)
{
    if ((uint8_t)level > (uint8_t)settings_.level) 
        return;

    char* dst = buffer_.data();
    uint8_t dt_size = utils::datetime_str(dst, buffer_.size());

    va_list arglist;
    va_start(arglist, format);
    vsnprintf(dst + dt_size, buffer_.size() - dt_size, format, arglist);
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