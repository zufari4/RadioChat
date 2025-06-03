#pragma once

#include "BatterySettings.h"
#include <chrono>
#include <atomic>

class Settings;

class Battery
{
public:
    Battery();
    ~Battery();
    void init(Settings& settings);
    void check();
    float getVoltage() const;

private:
    using Clock = std::chrono::system_clock;

    void loadSettings(Settings& settings);
    BatterySettings settings_;
    std::atomic<float> currentVoltage_;
    Clock::time_point nextCheckTime_;
};