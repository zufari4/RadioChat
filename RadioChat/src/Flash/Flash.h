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
    std::vector<std::string> getDirectories(const std::string& root);

private:
    State state_;
    FlashSettings settings_;
    void listDir(const char* dirname);
};