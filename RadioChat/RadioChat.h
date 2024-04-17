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

    Settings*   settings_;
    KeyHandler* keyHandler_;
    WiFiModule* wifi_;
    Display*    display_;
    Radio*      radio_;
    UI*         ui_;
};