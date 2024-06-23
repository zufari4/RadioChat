#include "Logger.h"
#include "../Flash/Flash.h"
#include "../Utils.h"
#include "../Configuration.h"
#include <HardwareSerial.h>
#include <SD.h>
#include <stdarg.h>
#include <algorithm>

Logger::Logger()
    : isInit_(false)
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

void Logger::init(const LoggerSettings& settings)
{
    settings_ = settings;
    std::string path = getPath();

    if (settings_.level != LogTraceLevel::None) {
        buffer_.resize(settings_.maxMessageSize + 25); // + date time

        if (settings_.logToSerial) initSerialLogging();

        if (!SD.exists(path.c_str())) {
            SD.mkdir(path.c_str());
        }

        std::string dt = utils::datetime_str();
        dt.erase(std::remove(dt.begin(), dt.end(), ':'), dt.end());
        dt.erase(std::remove(dt.begin(), dt.end(), '-'), dt.end());
        dt.erase(std::remove(dt.begin(), dt.end(), '.'), dt.end());
        dt.erase(std::remove(dt.begin(), dt.end(), ' '), dt.end());

        currentFile_ = path + "/" + dt + ".log";
        fs::File file = SD.open(currentFile_.c_str(), FILE_WRITE);
        if (file) {
            LOG_DBG("Created log file '%s'", currentFile_.c_str());
            file.close();
        }
        else {
            LOG_ERR("Can't create log file '%s'", currentFile_.c_str());
        }
    }

    LOG_INF("settings.level         : %d", (int)settings.level);
    LOG_INF("settings.logToSerial   : %s", utils::to_str(settings.logToSerial));
    LOG_INF("settings.logPath       : %s", path.c_str());
    LOG_INF("settings.maxCountLines : %u", settings.maxCountLines );
    LOG_INF("settings.maxCountLogs  : %u", settings.maxCountLogs  );
    LOG_INF("settings.maxMessageSize: %u", settings.maxMessageSize);
    
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
    offset += vsnprintf(dst + offset, buffer_.size() - offset, format, arglist);
    va_end(arglist);

    dst[offset++] = '\n';
    dst[offset] = '\0';

    if (settings_.logToSerial) {
        Serial.printf("%s", dst);
    }

    fs::File file = SD.open(currentFile_.c_str(), FILE_APPEND);
    if (file) {
        file.write((const uint8_t*)dst, offset);
        file.close();
    }
}

LogTraceLevel Logger::getLogLevel() const
{
    return settings_.level;
}

std::string Logger::getPath() const
{
    return STORAGE_DIR"/" + settings_.logPath;
}

std::string Logger::makeFilename() const
{
    return std::string();
}
