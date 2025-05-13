#pragma once

#include "FlashSettings.h"
#include <vector>
#include <string>
#include <mutex>

#define FLASH Flash::instance()

class Flash
{
public:  
    enum class State
    {
        NotInit,
        NoCard,
        Init,
    };

    Flash();
    ~Flash();
    static Flash& instance();
    void init(const FlashSettings& settings);
    void printInfo() const;
    std::string read(const std::string& filename);
    bool write(const std::string& filename, const std::string& content);
    bool append(const std::string& filename, const std::string& content);
    bool exists(const std::string& filename);
    void mkdir(const std::string& dirname);

private:
    State state_;
    FlashSettings settings_;
    std::recursive_mutex fsMutex_;

    void listDir(const char* dirname);
};