#include <string>
#include <stdint.h>
#include <functional>
#include "KeyHandler.h"
#include "Keyboard.h"
#include "Configuration.h"
#include "KeyMapRus.h"
#include "KeyMapEng.h"
#include "Logger.h"
#include "Utils.h"

KeyHandler::KeyHandler()
    : keyboard_(nullptr)
    , fnKey_(0)
    , enterKey_(0)
    , keyMap_(nullptr)
{
}

KeyHandler::~KeyHandler()
{
}

void KeyHandler::init(const KeyboardSettings& settings, CharCallback onChar, CmdCallback onCmd)
{
    onChar_ = onChar;
    onCmd_ = onCmd;
    using namespace std::placeholders;
    auto rawCallback = std::bind(&KeyHandler::onKeyUpRaw, this, _1, _2, _3);
    keyboard_ = new Keyboard(settings.pins.SH_LD, settings.pins.INH, settings.pins.QH, settings.pins.CLK, COUNT_KEYBOUARD_REGISTERS, rawCallback);
    keyboard_->init();
    setLanguage(settings.lang);
    fnKey_ = settings.fnKey;
    enterKey_ = settings.enterKey;
}

void KeyHandler::check()
{
    keyboard_->check();
}

void KeyHandler::onKeyUpRaw(uint8_t raw1, uint8_t raw2, uint8_t raw3)
{
    LOG("Buttons up: %u %u %u\n", raw1, raw2, raw3);
    bool isFnKey = raw1 == fnKey_ || raw2 == fnKey_ || raw3 == fnKey_;
    bool isEnterKey = raw1 == enterKey_ || raw2 == enterKey_ || raw3 == enterKey_;
    if (isFnKey && isEnterKey) {
        switchLang();
    }
    else if (isFnKey) {
        if (raw1 == fnKey_) {
            handleSymbol(keyMap_->altSymbol(raw2));
        }
        else {
            handleSymbol(keyMap_->altSymbol(raw1));
        }
    }
    else {
        handleSymbol(keyMap_->symbol(raw1));
    }
}

void KeyHandler::handleSymbol(uint16_t symbol)
{
    switch (symbol) {
    case 0                  : return;
    case KEY_CODE_BACKSPACE : handleCommand(KeyCommand::Backspace);    return;
    case KEY_CODE_ENTER     : handleCommand(KeyCommand::Enter);        return;
    case KEY_CODE_LEFT      : handleCommand(KeyCommand::Left);         return;
    case KEY_CODE_RIGHT     : handleCommand(KeyCommand::Right);        return;
    case KEY_CODE_UP        : handleCommand(KeyCommand::Up);           return;
    case KEY_CODE_DOWN      : handleCommand(KeyCommand::Down);         return;
    }

#if DEBUG_MODE == 1
    std::string str = utils::to_str(symbol);
    LOG("char: %s (0x%04X)\n", str.c_str(), symbol);
#endif
    onChar_(symbol);
}

void KeyHandler::handleCommand(KeyCommand cmd)
{
    LOG("%s: %s\n", __FUNCTION__, key_cmd_2_str(cmd));
    onCmd_(cmd);
}

void KeyHandler::switchLang()
{
    uint8_t nextLangNum = (uint8_t)keyMap_->getLang() + 1;
    if (nextLangNum >= (uint8_t)Language::Count) nextLangNum = 0;
    setLanguage((Language)nextLangNum);
    onCmd_(KeyCommand::ChangeLang);
}

void KeyHandler::setLanguage(Language lang)
{
    LOG("Set language to %s\n", lang2str(lang));
    if (keyMap_ != nullptr) {
        delete keyMap_;
    }
    switch (lang) {
    case Language::English: keyMap_ = new KeyMapEng(); break;
    default: keyMap_ = new KeyMapRus(); break;
    }
}

Language KeyHandler::getLang() const
{
    return keyMap_->getLang();
}