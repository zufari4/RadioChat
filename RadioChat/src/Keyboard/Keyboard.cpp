#include "Keyboard.h"
#include "../Logger/Logger.h"
#include "../Utils.h"
#include "../Settings/Settings.h"
#include <HardwareSerial.h>
#include <Arduino.h>

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::init(Settings& settings, KeyCallback onKeyDown, KeyCallback onKeyUp)
{
    loadSettings(settings);
    onKeyDown_ = onKeyDown;
    onKeyUp_   = onKeyUp;

    for (uint8_t key = 1; key <= settings_.maxKeyNum; ++key) {
        state_[key] = KeyState::Release;
    }

    pinMode(settings_.pins.CLK, OUTPUT);
    pinMode(settings_.pins.SH_LD, OUTPUT);
    pinMode(settings_.pins.QH, INPUT);
    digitalWrite(settings_.pins.CLK, LOW);
    digitalWrite(settings_.pins.SH_LD, HIGH);
}

void Keyboard::check()
{
    uint8_t key1 = 0;
    uint8_t key2 = 0;
    uint8_t key3 = 0;
    updatePressed(key1, key2, key3);
    KeyState currentState;
    
    for (uint8_t key = 1; key <= settings_.maxKeyNum; ++key) {
        currentState = key == key1 || key == key2 || key == key3 ? KeyState::Press : KeyState::Release;
        KeyState& prevState = state_[key];
        if (prevState != currentState) {
            if (currentState == KeyState::Press) onKeyDown_(key);
            else onKeyUp_(key);
            prevState = currentState;
        }
    }
}

void Keyboard::updatePressed(uint8_t& key1, uint8_t& key2, uint8_t& key3)
{
    digitalWrite(settings_.pins.SH_LD, LOW);
    delayMicroseconds(1);
    digitalWrite(settings_.pins.SH_LD, HIGH);

    uint8_t registerValue = 0;
    uint8_t totalBitNum = 0;
    uint8_t countPressed = 0;
    bool    isBitSet;
    //std::string bits;

    for (uint8_t i = 0; i < settings_.countRegisters; ++i) {
        registerValue = shiftIn(settings_.pins.QH, settings_.pins.CLK, MSBFIRST);
        //bits = utils::bits2str(registerValue);
        //Serial.printf("%u: %s ", i, bits.c_str());
        for (uint8_t b = 0; b < 8; ++b, ++totalBitNum) {
            isBitSet = registerValue & (1 << b);
            if (isBitSet) {
                continue;
            }
            ++countPressed;
            if (countPressed == 1) {
                key1 = totalBitNum + 1;
            }
            else if (countPressed == 2) {
                key2 = totalBitNum + 1;
            }
            else if (countPressed == 3) {
                key3 = totalBitNum + 1;
            }
            else {
                //Serial.printf("\n");
                return;
            }
        }
    }
    //Serial.printf("\n");
}

KeyState Keyboard::getState(uint8_t keyNum) const
{
    const auto it = state_.find(keyNum);
    return it == state_.end() ? KeyState::Release : it->second;
}

void Keyboard::loadSettings(Settings& settings)
{
    auto props = settings.keyboard();
    settings_.pins.SH_LD = Settings::get_i(eKeyboardPinShLd, props);
    settings_.pins.CLK = Settings::get_i(eKeyboardPinClk, props);
    settings_.pins.QH = Settings::get_i(eKeyboardPinQh, props);
    settings_.maxKeyNum = Settings::get_i(eKeyboardMaxKeyNum, props);
    settings_.countRegisters = Settings::get_i(eKeyboardCountRegisters, props);
    settings_.enterKey = Settings::get_i(eKeyboardEnterKey, props);
    settings_.fnKey = Settings::get_i(eKeyboardFnKey, props);
    settings_.lang = static_cast<Language>(Settings::get_i(eKeyboardLang, props));
}

const KeyboardSettings& Keyboard::getSettings() const
{
    return settings_;
}

