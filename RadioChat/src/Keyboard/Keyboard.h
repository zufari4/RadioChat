#pragma once
#include "KeyboardSettings.h"
#include "KeyState.h"
#include <stdint.h>
#include <functional>
#include <map>
#include <vector>

/* Class for SN74HC165 */
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
    void updatePressed(uint8_t& key1, uint8_t& key2, uint8_t& key3);

    KeyboardSettings settings_;
    KeyCallback onKeyDown_;
    KeyCallback onKeyUp_;
    std::map<uint8_t, KeyState> state_;
};