#pragma once

#include "KeyboardSettings.h"
#include "KeyCommand.h"
#include <functional>
#include <map>
#include <string>
#include "c_types.h"
#include <stdint.h>

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
    void onKeyUpRaw(uint8_t raw1, uint8_t raw2, uint8_t raw3);
    void setLanguage(Language lang);
    void switchLang();
    void handleSymbol(uint16_t symbol);
    void handleCommand(KeyCommand cmd);

    Keyboard* keyboard_;
    CharCallback onChar_;
    CmdCallback onCmd_;
    KeyMapBase* keyMap_;
    uint8_t fnKey_;
    uint8_t enterKey_;
};