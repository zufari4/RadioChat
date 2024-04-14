#include "HardwareSerial.h"
#include "Keyboard.h"
#include "Logger.h"
#include <Arduino.h>

Keyboard::Keyboard(uint8_t SH_LD, uint8_t INH, uint8_t QH, uint8_t CLK, uint8_t countRegisters, KeyCallback onKeyUp)
    : shPin_(SH_LD)
    , inhPin_(INH)
    , qhPin_(QH)
    , clkPin_(CLK)
    , countRegisters_(countRegisters)
    , onKeyUp_(onKeyUp)
    , key1_prev(0)
    , key2_prev(0)
    , key3_prev(0)
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::init()
{
    LOG("%s SH %u INH %u CLK %u QH %u\n", __FUNCTION__, shPin_, inhPin_, clkPin_, qhPin_);
    
    pinMode(shPin_, OUTPUT);
    pinMode(inhPin_   , OUTPUT);
    pinMode(clkPin_  , OUTPUT);
    pinMode(qhPin_ , INPUT);
    digitalWrite(clkPin_  , LOW);
    digitalWrite(shPin_, HIGH);
}

void Keyboard::check()
{
    get(key1_, key2_, key3_);
    if ((key1_ == 0 && key1_prev != 0) || (key2_ == 0 && key2_prev != 0) || (key3_ == 0 && key3_prev != 0)) {
        onKeyUp_(key1_prev, key2_prev, key3_prev);
        key1_prev = 0;
        key2_prev = 0;
        key3_prev = 0;
    }
    else {
        key1_prev = key1_;
        key2_prev = key2_;
        key3_prev = key3_;    
    }
}

void Keyboard::get(uint8_t& key1, uint8_t& key2, uint8_t& key3)
{
    digitalWrite(inhPin_   , HIGH);
    digitalWrite(shPin_, LOW);
    delay(5);
    digitalWrite(clkPin_  , HIGH);
    digitalWrite(shPin_, HIGH);
    digitalWrite(inhPin_   , LOW);
 
    uint8_t registerValue = 0;
    uint8_t bitNum = 0;
    uint8_t countPressed = 0;
    key1 = 0; key2 = 0; key3 = 0;

    for (uint8_t i = 0; i < countRegisters_; ++i) {
        registerValue = shiftIn(qhPin_, clkPin_, MSBFIRST);
        for (uint8_t b = 0; b < 8; ++b, ++bitNum) {
            bool isBitSet = registerValue & (1 << b);
            if (isBitSet) {
                continue;
            }
            ++countPressed;
            if (countPressed == 1) {
                key1 = bitNum + 1;
            }
            else if (countPressed == 2) {
                key2 = bitNum + 1;
            }
            else if (countPressed == 3) {
                key3 = bitNum + 1;
            }
            else {
                return;
            }
        }
    }
}

