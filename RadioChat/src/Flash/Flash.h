#pragma once

#include "FlashSettings.h"
#include <vector>
#include <string>

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
    void init(const FlashSettings& settings);
    void printInfo() const;
    bool exist(const std::string& path);
    bool createDir(const std::string& path);
    std::string read(const std::string& filename);
    bool create(const std::string& filename, const std::string& content);

private:
    State state_;
    FlashSettings settings_;
    
    void listDir(const char* dirname);
};