#include "Keyboard.h"
#include "../Logger/Logger.h"
#include "../Utils.h"
#include <HardwareSerial.h>
#include <Arduino.h>

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::init(const KeyboardSettings& settings, KeyCallback onKeyDown, KeyCallback onKeyUp)
{
    settings_  = settings_;
    onKeyDown_ = onKeyDown;
    onKeyUp_   = onKeyUp;

    for (uint8_t key = 1; key <= settings_.maxKeyNum; ++key) {
        state_[key] = KeyState::Release;
    }

    LOG_DBG("SH %u CLK %u QH %u", settings_.pins.SH_LD, settings_.pins.CLK, settings_.pins.QH);
    LOG_DBG("maxKeyNum %u countRegisters %u", settings_.maxKeyNum, settings_.countRegisters);

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

