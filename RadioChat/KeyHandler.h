#pragma once

#include "KeyboardSettings.h"
#include "KeyCommand.h"
#include <functional>
#include <map>
#include <string>
#include "c_types.h"
#include <stdint.h>
#include <memory>

class Keyboard;
class KeyMapBase;

class KeyHandler
{
public:
    using CharCallback = std::function<void(uint16_t symbol)>;
    using CmdCallback  = std::function<void(KeyCommand cmd)>;

    KeyHandler();
    ~KeyHandler();
    void init(const KeyboardSettings& settings, CharCallback onChar, CmdCallback onCmd);
    void check();
    Language getLang() const;

private:
    void onKeyDown(uint8_t keyNum);
    void onKeyUp(uint8_t keyNum);
    void setLanguage(Language lang);
    void switchLang();
    void handleSymbol(uint16_t symbol);
    void handleCommand(KeyCommand cmd);

    std::unique_ptr<Keyboard> keyboard_;
    CharCallback onChar_;
    CmdCallback onCmd_;
    std::unique_ptr<KeyMapBase> keyMap_;
    uint8_t fnKey_;
    uint8_t enterKey_;
    bool fnKeyPressed_;
    bool fnKeyHandled_;
};