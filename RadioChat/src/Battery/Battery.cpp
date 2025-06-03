#include "Battery.h"
#include "../Logger/Logger.h"
#include "../Settings/Settings.h"
#include <Arduino.h>

Battery::Battery()
    : currentVoltage_(0.0f)
{
}

Battery::~Battery()
{
}

void Battery::init(Settings &settings)
{
    LOG_INF("-- Initialize battery --");
    loadSettings(settings);
    nextCheckTime_ = Clock::now();
    check();
    LOG_INF("Voltage: %.2f", currentVoltage_.load());
}

void Battery::check()
{
    if (Clock::now() >= nextCheckTime_) {
        nextCheckTime_ = Clock::now() + std::chrono::milliseconds(settings_.checkInterval);
        float adc = analogRead(settings_.pinVotage);
        currentVoltage_ = ((adc * settings_.maxBatteryVoltage) / settings_.maxADC) + settings_.cFactor;
    }
}

float Battery::getVoltage() const
{
    return currentVoltage_;
}

void Battery::loadSettings(Settings& settings)
{
    auto props = settings.battery();
    settings_.cFactor = Settings::get_f(eBatteryCFactor, props);
    settings_.pinVotage = Settings::get_i(eBatteryPinVoltage, props);
    settings_.maxBatteryVoltage = Settings::get_f(eBatteryMaxVoltage, props);
    settings_.checkInterval = Settings::get_i(eBatteryCheckInterval, props);
    settings_.maxADC = Settings::get_f(eBatteryMaxADC, props);
}
