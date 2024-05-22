#pragma once
#include "KeyboardSettings.h"
#include "KeyState.h"
#include <stdint.h>
#include <functional>
#include <map>
#include <vector>

/* Class for 74HC165 */
class Keyboard
{
public:
    using KeyCallback = std::function<void(uint8_t keyNum)>;

    Keyboard();
    ~Keyboard();
    void init(const KeyboardSettings& settings, KeyCallback onKeyDown, KeyCallback onKeyUp);
    void check();
    KeyState getState(uint8_t keyNum) const;
    
private:
    void updatePressed();

    KeyboardSettings settings_;
    KeyCallback onKeyDown_;
    KeyCallback onKeyUp_;
    std::map<uint8_t, KeyState> state_;
    uint8_t key1_, key2_, key3_;
    uint8_t registerValue_;
    uint8_t bitNum_;
    uint8_t countPressed_;
    uint8_t countKeys_;
};