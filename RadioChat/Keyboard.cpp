#include "HardwareSerial.h"
#include "Keyboard.h"
#include "Logger.h"
#include <Arduino.h>

Keyboard::Keyboard()
    : key1_(0)
    , key2_(0)
    , key3_(0)
    , registerValue_(0)
    , bitNum_(0)
    , countPressed_(0)
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::init(const KeyboardSettings& settings, KeyCallback onKeyDown, KeyCallback onKeyUp)
{
    LOG("Initialize keyboard\n");

    settings_  = settings_;
    onKeyDown_ = onKeyDown;
    onKeyUp_   = onKeyUp;

    for (uint8_t key = 1; key <= settings_.maxKeyNum; ++key) {
        state_[key] = KeyState::Release;
    }

    LOG("SH %u INH %u CLK %u QH %u\n", settings_.pins.SH_LD, settings_.pins.INH, settings_.pins.CLK, settings_.pins.QH);
    LOG("maxKeyNum %u countRegisters %u\n", settings_.maxKeyNum, settings_.countRegisters);

    pinMode(settings_.pins.SH_LD, OUTPUT);
    pinMode(settings_.pins.INH, OUTPUT);
    pinMode(settings_.pins.CLK, OUTPUT);
    pinMode(settings_.pins.QH, INPUT);
    digitalWrite(settings_.pins.CLK, LOW);
    digitalWrite(settings_.pins.SH_LD, HIGH);
}

void Keyboard::check()
{
    updatePressed();
    KeyState currentState;
    
    for (uint8_t key = 1; key <= settings_.maxKeyNum; ++key) {
        currentState = key == key1_ || key == key2_ || key == key3_ ? KeyState::Press : KeyState::Release;
        KeyState& prevState = state_[key];
        if (prevState != currentState) {
            if (currentState == KeyState::Press) onKeyDown_(key);
            else onKeyUp_(key);
            prevState = currentState;
        }
    }
}

void Keyboard::updatePressed()
{
    digitalWrite(settings_.pins.INH, HIGH);
    digitalWrite(settings_.pins.SH_LD , LOW);
    delay(5);
    digitalWrite(settings_.pins.CLK, HIGH);
    digitalWrite(settings_.pins.SH_LD , HIGH);
    digitalWrite(settings_.pins.INH, LOW);
 
    registerValue_ = 0;
    bitNum_ = 0;
    countPressed_ = 0;
    key1_ = 0; key2_ = 0; key3_ = 0;
    bool isBitSet;

    for (uint8_t i = 0; i < settings_.countRegisters; ++i) {
        registerValue_ = shiftIn(settings_.pins.QH, settings_.pins.CLK, MSBFIRST);
        for (uint8_t b = 0; b < 8; ++b, ++bitNum_) {
            isBitSet = registerValue_ & (1 << b);
            if (isBitSet) {
                continue;
            }
            ++countPressed_;
            if (countPressed_ == 1) {
                key1_ = bitNum_ + 1;
            }
            else if (countPressed_ == 2) {
                key2_ = bitNum_ + 1;
            }
            else if (countPressed_ == 3) {
                key3_ = bitNum_ + 1;
            }
            else {
                return;
            }
        }
    }
}

KeyState Keyboard::getState(uint8_t keyNum) const
{
    const auto it = state_.find(keyNum);
    return it == state_.end() ? KeyState::Release : it->second;
}

