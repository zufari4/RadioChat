#pragma once

#include "KeyCommand.h"
#include <stdint.h>
#include <string>


class Flash;
class Settings;
class Esp;
class KeyHandler;
class Display;
class Radio;
class LedIndicator;
class UI;

class RadioChat
{
public:
    RadioChat();
    ~RadioChat();
    void init();
    void loop();

private:
    void onChar(uint16_t symbol);
    void onKeyCommand(KeyCommand cmd);
    void onNewMessage(const std::string& text, uint8_t msgID);
    void onMessageDelivery(uint16_t dest, uint8_t msgID);
    void onPingDone(uint16_t address, uint32_t delay);
    
    Settings*     settings_;
    Flash*        flash_;
    Esp*          esp_;
    KeyHandler*   keyHandler_;
    Display*      display_;
    Radio*        radio_;
    UI*           ui_;
    LedIndicator* ledIndicator_;
};