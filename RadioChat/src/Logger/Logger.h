#pragma once

#include "LogTraceLevel.h"
#include <vector>
#include <memory>

#define LOG_ERR(...) Logger::instance().log(LogTraceLevel::Error, __VA_ARGS__)
#define LOG_INF(...) Logger::instance().log(LogTraceLevel::Info, __VA_ARGS__)
#define LOG_DBG(...) Logger::instance().log(LogTraceLevel::Debug, __VA_ARGS__)

class Settings;

class Logger
{
public:
    Logger();
    ~Logger();

    void initSerialLogging();
    void init(Settings& settings);
    static Logger& instance();
    void log(LogTraceLevel level, const char* format, ...);
    LogTraceLevel getLogLevel() const;
    
private:
    std::string getPath() const;
    std::string makeFilename() const;
    void createFile();
    
    bool isInit_;
    bool serialIsInit_;
    LogTraceLevel level_;
    size_t maxMessageSize_;
    size_t maxCountLines_;
    size_t maxCountLogs_;
    bool logToSerial_;
    bool logToFile_;
    std::string logPath_;
    std::vector<char> buffer_;
    std::mutex mtx_;
    bool isFileOpen_;
    std::string filename_;
};