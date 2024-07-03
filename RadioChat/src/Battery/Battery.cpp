#include "Battery.h"
#include "../Logger/Logger.h"
#include <Arduino.h>

Battery::Battery()
    : currentVoltage_(0.0f)
{
}

Battery::~Battery()
{
}

void Battery::init(const BatterySettings &settings)
{
    LOG_INF("-- Initialize keyboard --");
    settings_ = settings;
    LOG_DBG("Pin voltage: %u Max Voltage: %.2f Check interval %u Correctioin factor %.2f", 
        settings_.pinVotage, settings_.maxBatteryVoltage, settings_.checkInterval, settings_.cFactor);
    nextCheckTime_ = Clock::now();
}

void Battery::check()
{
    if (Clock::now() >= nextCheckTime_) {
        nextCheckTime_ = Clock::now() + std::chrono::milliseconds(settings_.checkInterval);
        float adc = analogRead(settings_.pinVotage);
        currentVoltage_ = ((adc * settings_.maxBatteryVoltage) / settings_.maxADC) + settings_.cFactor;
        LOG_DBG("Voltage: %.2f", currentVoltage_.load());
    }
}

float Battery::getVoltage() const
{
    return currentVoltage_;
}
