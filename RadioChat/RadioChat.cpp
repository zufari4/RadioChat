#include <string>
#include "RadioChat.h"
#include "Logger.h"
#include "Settings.h"
#include "KeyHandler.h"
#include "WiFi.h"
#include "Display.h"
#include "UI.h"

RadioChat::RadioChat()
    : settings_(nullptr)
    , keyHandler_(nullptr)
    , wifi_(nullptr)
    , display_(nullptr)
    , ui_(nullptr)
{

}

RadioChat::~RadioChat()
{

}

void RadioChat::init()
{
    INIT_LOG();

    settings_   = new Settings(SETTINGS_FILENAME);
    keyHandler_ = new KeyHandler();
    wifi_       = new WiFiModule();
    display_    = new Display();
    ui_         = new UI();

    KeyboardSettings kbSettings = settings_->keyboard();
    keyHandler_->init(kbSettings, 
                      std::bind(&RadioChat::onChar, this, std::placeholders::_1), 
                      std::bind(&RadioChat::onKeyCommand, this, std::placeholders::_1));

    WiFiSettings wifiSettings = settings_->wifi();
    wifi_->init(wifiSettings);

    DisplaySettings dispSettings = settings_->display();
    display_->init(dispSettings);

    UISettings uiSettings = settings_->ui();
    ui_->init(uiSettings, display_);
}

void RadioChat::loop()
{
    keyHandler_->check();
    ui_->draw();
}

void RadioChat::onChar(uint16_t symbol)
{
    ui_->onChar(symbol);
}

void RadioChat::onKeyCommand(KeyCommand cmd)
{
    ui_->onKeyCommand(cmd);
}