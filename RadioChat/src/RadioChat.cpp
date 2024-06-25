#include "RadioChat.h"
#include "Configuration.h"
#include "Logger/Logger.h"
#include "Settings.h"
#include "Flash/Flash.h"
#include "Esp/Esp.h"
#include "Keyboard/KeyHandler.h"
#include "Display/Display.h"
#include "Radio/Radio.h"
#include "Led/LedIndicator.h"
#include "Sound/Sound.h"
#include "UI/UI.h"
#include "QeueMessage/QeueMessageAcceptMessage.h"
#include "QeueMessage/QeueMessageDeliveryMessage.h"
#include "QeueMessage/QeueMessageKeyboardCommand.h"
#include "QeueMessage/QeueMessagePingDone.h"
#include "QeueMessage/QeueMessageTypingChar.h"
#include <SD.h>
#include <stdexcept>

RadioChat::RadioChat()
    : workFlag_(false)
    , settings_(nullptr)
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

#if DEBUG_MODE == 1
    // in debug always print start info
    // for it need serial
    // else init serial by logger settings
    Logger::instance().initSerialLogging();
#endif

    flash_      = new Flash();
    settings_   = new Settings();
    esp_        = new Esp();
    keyHandler_ = new KeyHandler();
    display_    = new Display();
    radio_      = new Radio();
    ui_         = new UI();
    ledIndicator_ = new LedIndicator();
    sound_       = new Sound();

    FlashSettings flashSettings;
    flash_->init(flashSettings);

    if (!SD.exists(STORAGE_DIR)) {
        SD.mkdir(STORAGE_DIR);
    }

    settings_->init(SETTINGS_FILENAME);

    LoggerSettings loggerSettings = settings_->logger();
    Logger::instance().init(loggerSettings);
    flash_->printInfo(); // need logger for print

    EspSettings espSettings = settings_->esp();
    esp_->init(espSettings);

    KeyboardSettings  keybSettings = settings_->keyboard();
    keyHandler_->init(keybSettings, 
                      std::bind(&RadioChat::pushTypingChar, this, _1), 
                      std::bind(&RadioChat::pushKeyboardCommand, this, _1));

    DisplaySettings dispSettings = settings_->display();
    display_->init(dispSettings);

    RadioSettings radioSettings = settings_->radio();
    radio_->init(radioSettings,
                 std::bind(&RadioChat::pushAcceptMessage, this, _1, _2), 
                 std::bind(&RadioChat::pushDeliveryMessage, this, _1, _2),
                 std::bind(&RadioChat::pushPingDone, this, _1, _2));

    LedSettings ledSettings = settings_->led();
    ledIndicator_->init(ledSettings);

    UISettings uiSettings = settings_->ui();
    ui_->init(uiSettings, display_);

    SoundSettings soundSettings = settings_->sound();
    sound_->init(soundSettings);

    workFlag_ = true;
    svcThread_ = std::thread(&RadioChat::svc, this);

    sound_->play(Melody::Name::Nokia);
}

void RadioChat::loop()
{
    radio_->check();
    keyHandler_->check();
    ledIndicator_->check();
    ui_->draw();
}

void RadioChat::svc()
{
    while (workFlag_) {
        checkQeue();
    }
}

void RadioChat::checkQeue()
{
    auto msg = messageQeue_.dequeue();
    switch (msg->getType())
    {
    case QeueMessageType::AcceptMessage: {
        auto m = static_cast<QeueMessageAcceptMessage*>(msg.get());
        sound_->play(Melody::Name::PackmanShort);
        break;
    }
    case QeueMessageType::DeliveryMessage: {
        auto m = static_cast<QeueMessageDeliveryMessage*>(msg.get());
        break;
    }   
    case QeueMessageType::KeyboardCommand: {
        auto m = static_cast<QeueMessageKeyboardCommand*>(msg.get());
        ui_->onKeyCommand(m->getCommand());
        break;
    }
    case QeueMessageType::PingDone: {
        auto m = static_cast<QeueMessagePingDone*>(msg.get());
        break;
    }
    case QeueMessageType::TypingChar: {
        auto m = static_cast<QeueMessageTypingChar*>(msg.get());
        ui_->onChar(m->getCode());
        break;
    }
    default:
        break;
    }
}

void RadioChat::pushTypingChar(uint16_t code)
{
    auto msg = std::make_unique<QeueMessageTypingChar>(code);
    messageQeue_.enqueue(std::move(msg));
}

void RadioChat::pushKeyboardCommand(KeyCommand cmd)
{
    auto msg = std::make_unique<QeueMessageKeyboardCommand>(cmd);
    messageQeue_.enqueue(std::move(msg));
}

void RadioChat::pushAcceptMessage(const std::string& text, uint8_t msgID)
{
    auto msg = std::make_unique<QeueMessageAcceptMessage>(text, msgID);
    messageQeue_.enqueue(std::move(msg));
}

void RadioChat::pushDeliveryMessage(uint16_t address, uint8_t msgID)
{
    auto msg = std::make_unique<QeueMessageDeliveryMessage>(address, msgID);
    messageQeue_.enqueue(std::move(msg));
}

void RadioChat::pushPingDone(uint16_t address, uint32_t delay)
{
    auto msg = std::make_unique<QeueMessagePingDone>(address, delay);
    messageQeue_.enqueue(std::move(msg));
}