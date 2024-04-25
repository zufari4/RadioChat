#include <string>
#include "RadioChat.h"
#include "Logger.h"
#include "Settings.h"
#include "Flash.h"
#include "Esp.h"
#include "KeyHandler.h"
#include "Display.h"
#include "Radio.h"
#include "LedIndicator.h"
#include "UI.h"

RadioChat::RadioChat()
    : settings_(nullptr)
    , flash_(nullptr)
    , esp_(nullptr)
    , keyHandler_(nullptr)
    , display_(nullptr)
    , radio_(nullptr)
    , ui_(nullptr)
    , ledIndicator_(nullptr)
{

}

RadioChat::~RadioChat()
{

}

void RadioChat::init()
{   
    using namespace std::placeholders;

    flash_      = new Flash();
    settings_   = new Settings();
    esp_        = new Esp();
    keyHandler_ = new KeyHandler();
    display_    = new Display();
    radio_      = new Radio();
    ui_         = new UI();
    ledIndicator_ = new LedIndicator();

    FlashSettings flashSettings;
    flash_->init(flashSettings);

    settings_->init(SETTINGS_FILENAME, flash_);

    LoggerSettings loggerSettings = settings_->logger();
    Logger::instance().init(loggerSettings);

    EspSettings espSettings = settings_->esp();
    esp_->init(espSettings);

    KeyboardSettings  keybSettings = settings_->keyboard();
    keyHandler_->init(keybSettings, 
                      std::bind(&RadioChat::onChar, this, _1), 
                      std::bind(&RadioChat::onKeyCommand, this, _1));

    DisplaySettings dispSettings = settings_->display();
    display_->init(dispSettings);

    RadioSettings radioSettings = settings_->radio();
    radio_->init(radioSettings,
                 std::bind(&RadioChat::onNewMessage, this, _1, _2), 
                 std::bind(&RadioChat::onMessageDelivery, this, _1, _2),
                 std::bind(&RadioChat::onPingDone, this, _1, _2));

    LedSettings ledSettings = settings_->led();
    ledIndicator_->init(ledSettings);

    UISettings uiSettings = settings_->ui();
    ui_->init(uiSettings, display_);
}

void RadioChat::loop()
{
    radio_->check();
    keyHandler_->check();
    ledIndicator_->check();
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