#pragma once

#include "LedSettings.h"
#include <chrono>

class Settings;

class LedIndicator
{
public:
    LedIndicator();
    ~LedIndicator();
    void init(Settings& settings);
    void check();

private:
    using Clock = std::chrono::system_clock;
    void loadSettings(Settings& settings);

    LedSettings settings_;
    bool state_;
    Clock::time_point nextSwitchTime_;
};