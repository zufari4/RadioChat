#pragma once

#include "LoggerSettings.h"
#include <vector>

#define LOG_ERR(...) Logger::instance().log(LogTraceLevel::Error, __VA_ARGS__)
#define LOG_INF(...) Logger::instance().log(LogTraceLevel::Info, __VA_ARGS__)
#define LOG_DBG(...) Logger::instance().log(LogTraceLevel::Debug, __VA_ARGS__)

class Flash;

class Logger
{
public:
    Logger();
    ~Logger();
    void init(const LoggerSettings& settings, Flash* flash);
    static Logger& instance();
    void log(LogTraceLevel level, const char* format, ...);
    LogTraceLevel getLogLevel() const;
    
private:
    LoggerSettings settings_;
    std::vector<char> buffer_;
    Flash* flash_;
    bool isInit_;
};