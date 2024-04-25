#include "LedIndicator.h"

LedIndicator::LedIndicator()
{

}

LedIndicator::~LedIndicator()
{

}

void LedIndicator::init(const LedSettings& settings)
{
    LOG_INF("Init led indicator\n");
    settings_ = settings;
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