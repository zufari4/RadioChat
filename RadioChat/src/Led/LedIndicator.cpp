#include "LedIndicator.h"
#include "../Logger/Logger.h"
#include "../Settings/Settings.h"
#include <Arduino.h>

LedIndicator::LedIndicator()
{

}

LedIndicator::~LedIndicator()
{

}

void LedIndicator::init(Settings& settingsMgr)
{
    LOG_INF("--- Init led indicator ---");
    loadSettings(settingsMgr);
    pinMode(settings_.pins.on, OUTPUT);
    digitalWrite(settings_.pins.on, LOW);
    state_ = false;
    nextSwitchTime_ = Clock::now() + std::chrono::milliseconds(settings_.interval);
}

void LedIndicator::check()
{
    if (Clock::now() >= nextSwitchTime_) {
        nextSwitchTime_ = Clock::now() + std::chrono::milliseconds(settings_.interval);
        digitalWrite(settings_.pins.on, state_ ? LOW : HIGH);
        state_ = !state_;
    }
}

void LedIndicator::loadSettings(Settings& settings)
{
    auto props = settings.led();
    settings_.pins.on = Settings::get_i(eLedPinOn, props);
    settings_.interval = Settings::get_i(eLedInterval, props);
}
