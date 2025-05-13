#include "Logger.h"
#include "../Flash/Flash.h"
#include "../Utils.h"
#include "../Configuration.h"
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

void Logger::init(const LoggerSettings& settings)
{
    settings_ = settings;
    std::string path = getPath();

    if (settings_.level != LogTraceLevel::None) {
        buffer_.resize(settings_.maxMessageSize + 25); // + date time

        if (settings_.logToSerial) {
            initSerialLogging();
        }
        if (settings_.logToFile) {
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
    LOG_INF("settings.level         : %d", (int)settings.level);
    LOG_INF("settings.logToSerial   : %s", utils::to_str(settings.logToSerial));
    LOG_INF("settings.logPath       : %s", path.c_str());
    LOG_INF("settings.maxCountLines : %u", settings.maxCountLines );
    LOG_INF("settings.maxCountLogs  : %u", settings.maxCountLogs  );
    LOG_INF("settings.maxMessageSize: %u", settings.maxMessageSize);
}

void Logger::log(LogTraceLevel level, const char* format, ...)
{
    if (!isInit_ && !serialIsInit_) 
        return;
    if ((uint8_t)level > (uint8_t)settings_.level) 
        return;
        
    std::lock_guard guard(mtx_);
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

    if (isFileOpen_) {
        FLASH.append(filename_, dst);
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
