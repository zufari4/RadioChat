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
    static std::string read(const std::string& filename);
    static bool create(const std::string& filename, const std::string& content);

private:
    State state_;
    FlashSettings settings_;
    
    void listDir(const char* dirname);
};