#include <string>
#include "RadioChat.h"
#include "Logger.h"
#include "Settings.h"
#include "KeyHandler.h"
#include "WiFi.h"
#include "Display.h"
#include "Radio.h"
#include "UI.h"

RadioChat::RadioChat()
    : settings_(nullptr)
    , keyHandler_(nullptr)
    , wifi_(nullptr)
    , display_(nullptr)
    , radio_(nullptr)
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
    radio_      = new Radio();
    ui_         = new UI();
    
    using namespace std::placeholders;

    KeyboardSettings kbSettings = settings_->keyboard();
    keyHandler_->init(kbSettings, 
                      std::bind(&RadioChat::onChar, this, _1), 
                      std::bind(&RadioChat::onKeyCommand, this, _1));

    WiFiSettings wifiSettings = settings_->wifi();
    wifi_->init(wifiSettings);

    DisplaySettings dispSettings = settings_->display();
    display_->init(dispSettings);

    RadioSettings radioSettings = settings_->radio();
    radio_->init(radioSettings,
                 std::bind(&RadioChat::onNewMessage, this, _1, _2), 
                 std::bind(&RadioChat::onMessageDelivery, this, _1, _2),
                 std::bind(&RadioChat::onPingDone, this, _1, _2));

    UISettings uiSettings = settings_->ui();
    ui_->init(uiSettings, display_);
}

void RadioChat::loop()
{
    radio_->check();
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

void RadioChat::onNewMessage(const std::string& text, uint8_t msgID)
{

}

void RadioChat::onMessageDelivery(uint16_t dest, uint8_t msgID)
{

}

void RadioChat::onPingDone(uint16_t address, uint32_t delay)
{

}