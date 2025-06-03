#include "Logger.h"
#include "../Flash/Flash.h"
#include "../Utils.h"
#include "../Configuration.h"
#include "../Settings/Settings.h"
#include <HardwareSerial.h>
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

void Logger::init(Settings& settings)
{
    {
        auto props = settings.logger();
        logPath_ = Settings::get_s(eLogPath, props);
        level_ = (LogTraceLevel)Settings::get_i(eLogTraceLevel, props);
        logToSerial_ = Settings::get_b(eLogToSerial, props);
        logToFile_ = Settings::get_b(eLogToFile, props);
        maxMessageSize_ = Settings::get_i(eLogMaxMessageSize, props);
        maxCountLines_ = Settings::get_i(eLogMaxCountLines, props);
        maxCountLogs_ = Settings::get_i(eLogMaxCountLogs, props);
    }
    std::string path = getPath();

    if (level_ != LogTraceLevel::None) {
        buffer_.resize(maxMessageSize_ + 25); // + date time

        if (logToSerial_) {
            initSerialLogging();
        }
        if (logToFile_) {
            if (FLASH.exists(path)) {
                LOG_DBG("Log path %s already exists", path.c_str());
            }
            else {
                LOG_DBG("Log path %s not exists. Create", path.c_str());
                FLASH.mkdir(path);
            }
            createFile();
        }
    }
    isInit_ = true;
}

void Logger::log(LogTraceLevel level, const char* format, ...)
{
    if (!isInit_ && !serialIsInit_) 
        return;
    if ((uint8_t)level > (uint8_t)level_) 
        return;
        
    std::lock_guard guard(mtx_);
    char* dst = buffer_.data();
    uint8_t offset = utils::datetime_str(dst, buffer_.size());
    dst[offset++] = ' ';

    offset += snprintf(dst + offset, buffer_.size() - offset, "[%s] ", traceLvlToLog(level).data());

    va_list arglist;
    va_start(arglist, format);
    offset += vsnprintf(dst + offset, buffer_.size() - offset, format, arglist);
    va_end(arglist);

    dst[offset++] = '\n';
    dst[offset] = '\0';

    if (logToSerial_) {
        Serial.printf("%s", dst);
    }

    if (isFileOpen_) {
        FLASH.append(filename_, dst);
    }
}

LogTraceLevel Logger::getLogLevel() const
{
    return level_;
}

std::string Logger::getPath() const
{
    return STORAGE_DIR"/" + logPath_;
}

std::string Logger::makeFilename() const
{
    return std::string();
}

void Logger::createFile()
{
    std::string dt = utils::datetime_str();
    dt.erase(std::remove(dt.begin(), dt.end(), ':'), dt.end());
    dt.erase(std::remove(dt.begin(), dt.end(), '-'), dt.end());
    dt.erase(std::remove(dt.begin(), dt.end(), '.'), dt.end());
    dt.erase(std::remove(dt.begin(), dt.end(), ' '), dt.end());

    filename_ = getPath() + "/" + dt + ".log";
    if (FLASH.write(filename_, "")) {
        isFileOpen_ = true;
        LOG_DBG("Log file %s created", filename_.c_str());
    }
    else {
        LOG_ERR("Failed to create log file %s", filename_.c_str());
        isFileOpen_ = false;
    }
}
