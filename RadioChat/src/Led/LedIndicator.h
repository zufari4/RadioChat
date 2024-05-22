#pragma once

#include "LedSettings.h"
#include <chrono>

class LedIndicator
{
public:
    LedIndicator();
    ~LedIndicator();
    void init(const LedSettings& settings);
    void check();

private:
    using Clock = std::chrono::system_clock;

    LedSettings settings_;
    bool state_;
    Clock::time_point nextSwitchTime_;
};