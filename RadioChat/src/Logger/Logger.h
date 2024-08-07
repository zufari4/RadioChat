#pragma once

#include "LoggerSettings.h"
#include <vector>
#include <memory>

#define LOG_ERR(...) Logger::instance().log(LogTraceLevel::Error, __VA_ARGS__)
#define LOG_INF(...) Logger::instance().log(LogTraceLevel::Info, __VA_ARGS__)
#define LOG_DBG(...) Logger::instance().log(LogTraceLevel::Debug, __VA_ARGS__)

namespace fs
{
    class File;
}

class Logger
{
public:
    Logger();
    ~Logger();

    void initSerialLogging();
    void init(const LoggerSettings& settings);
    static Logger& instance();
    void log(LogTraceLevel level, const char* format, ...);
    LogTraceLevel getLogLevel() const;
    
private:
    std::string getPath() const;
    std::string makeFilename() const;
    void createFile();
    
    bool isInit_;
    bool serialIsInit_;
    LoggerSettings settings_;
    std::vector<char> buffer_;
    std::mutex mtx_;
    std::unique_ptr<fs::File> currentFile_;
};