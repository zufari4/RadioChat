#pragma once

#include "KeyCommand.h"
#include <stdint.h>
#include <string>

class Settings;
class KeyHandler;
class WiFiModule;
class Display;
class Radio;
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
    
    Settings*   settings_;
    KeyHandler* keyHandler_;
    WiFiModule* wifi_;
    Display*    display_;
    Radio*      radio_;
    UI*         ui_;
};