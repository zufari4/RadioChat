#pragma once

#include "BatterySettings.h"
#include <chrono>
#include <atomic>

class Battery
{
public:
    Battery();
    ~Battery();
    void init(const BatterySettings& settings);
    void check();
    float getVoltage() const;

private:
    using Clock = std::chrono::system_clock;

    BatterySettings settings_;
    std::atomic<float> currentVoltage_;
    Clock::time_point nextCheckTime_;
};