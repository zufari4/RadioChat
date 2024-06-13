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
#include "QeueMessage/MessageQeueManager.h"
#include <string>


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
    qeueManager_ = new MessageQeueManager();

    FlashSettings flashSettings;
    flash_->init(flashSettings);

    if (!flash_->exist(STORAGE_DIR)) {
        flash_->createDir(STORAGE_DIR);
    }

    settings_->init(SETTINGS_FILENAME, flash_);

    LoggerSettings loggerSettings = settings_->logger();
    Logger::instance().init(loggerSettings, flash_);
    flash_->printInfo(); // need logger for print

    EspSettings espSettings = settings_->esp();
    esp_->init(espSettings);

    KeyboardSettings  keybSettings = settings_->keyboard();
    keyHandler_->init(keybSettings, 
                      std::bind(&MessageQeueManager::pushTypingChar, qeueManager_, _1), 
                      std::bind(&MessageQeueManager::pushKeyboardCommand, qeueManager_, _1));

    DisplaySettings dispSettings = settings_->display();
    display_->init(dispSettings);

    RadioSettings radioSettings = settings_->radio();
    radio_->init(radioSettings,
                 std::bind(&MessageQeueManager::pushAcceptMessage, qeueManager_, _1, _2), 
                 std::bind(&MessageQeueManager::pushDeliveryMessage, qeueManager_, _1, _2),
                 std::bind(&MessageQeueManager::pushPingDone, qeueManager_, _1, _2));

    LedSettings ledSettings = settings_->led();
    ledIndicator_->init(ledSettings);

    UISettings uiSettings = settings_->ui();
    ui_->init(uiSettings, display_);

    SoundSettings soundSettings = settings_->sound();
    sound_->init(soundSettings);

    sound_->play(Melody::Name::Nokia);

    workFlag_ = true;
    svcThread_ = std::thread(&RadioChat::svc, this);
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
        auto msg = qeueManager_->pop();
        switch (msg->getType())
        {
        case QeueMessageType::AcceptMessage:
            /* code */
            break;
        case QeueMessageType::DeliveryMessage:
            /* code */
            break;    
        case QeueMessageType::KeyboardCommand:
            /* code */
            break;
        case QeueMessageType::PingDone:
            /* code */
            break;
        case QeueMessageType::TypingChar:
            /* code */
            break;
        default:
            break;
        }
    }
}
