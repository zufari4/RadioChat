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
    : fnKey_(0)
    , enterKey_(0)
    , fnKeyPressed_(false)
{
}

KeyHandler::~KeyHandler()
{
}

void KeyHandler::init(const KeyboardSettings& settings, CharCallback onChar, CmdCallback onCmd)
{
    onChar_ = onChar;
    onCmd_  = onCmd;
    auto keyDownCb = std::bind(&KeyHandler::onKeyDown, this, std::placeholders::_1);
    auto keyUpCb = std::bind(&KeyHandler::onKeyUp, this, std::placeholders::_1);
    keyboard_ = std::make_unique<Keyboard>();
    keyboard_->init(settings, keyDownCb, keyUpCb);
    setLanguage(settings.lang);
    fnKey_ = settings.fnKey;
    enterKey_ = settings.enterKey;
    LOG("FN key %u Enter key %u\n", fnKey_, enterKey_);
}

void KeyHandler::check()
{
    keyboard_->check();
}

void KeyHandler::onKeyDown(uint8_t keyNum)
{
    LOG("Button down: %u\n", keyNum);

    if (keyNum == fnKey_) {
        fnKeyPressed_ = true;
        fnKeyHandled_ = false;
    }
    else if (fnKeyPressed_ && keyNum == enterKey_) {
        switchLang();
        fnKeyHandled_ = true;
    }
    else if (fnKeyPressed_) {
        handleSymbol(keyMap_->altSymbol(keyNum));
        fnKeyHandled_ = true;
    }
    else {
        handleSymbol(keyMap_->symbol(keyNum));
        fnKeyHandled_ = false;
    }
}

void KeyHandler::onKeyUp(uint8_t keyNum)
{
    LOG("Button up: %u\n", keyNum);

    if (keyNum == fnKey_) {
        fnKeyPressed_ = false;
        if (!fnKeyHandled_) {
            handleCommand(KeyCommand::Escape); 
        }
    }
}

void KeyHandler::handleSymbol(uint16_t symbol)
{
    switch (symbol) {
    case 0                 : return;
    case KEY_CODE_BACKSPACE: handleCommand(KeyCommand::Backspace);    return;
    case KEY_CODE_ENTER    : handleCommand(KeyCommand::Enter);        return;
    case KEY_CODE_LEFT     : handleCommand(KeyCommand::Left);         return;
    case KEY_CODE_RIGHT    : handleCommand(KeyCommand::Right);        return;
    case KEY_CODE_UP       : handleCommand(KeyCommand::Up);           return;
    case KEY_CODE_DOWN     : handleCommand(KeyCommand::Down);         return;
    case KEY_CODE_FN       : handleCommand(KeyCommand::Escape);       return;
    }

#if DEBUG_MODE == 1
    std::string str = utils::to_str(symbol);
    LOG("char: '%s' (0x%04X)\n", str.c_str(), symbol);
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

    switch (lang) {
    case Language::English: keyMap_ = std::make_unique<KeyMapEng>(); break;
    default: keyMap_ = std::make_unique<KeyMapRus>(); break;
    }
}

Language KeyHandler::getLang() const
{
    return keyMap_->getLang();
}