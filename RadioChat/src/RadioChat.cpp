#include "RadioChat.h"
#include "Configuration.h"
#include "Logger/Logger.h"
#include "Settings/Settings.h"
#include "Flash/Flash.h"
#include "Esp/Esp.h"
#include "Keyboard/KeyHandler.h"
#include "Display/Display.h"
#include "Radio/Radio.h"
#include "Led/LedIndicator.h"
#include "Sound/Sound.h"
#include "Battery/Battery.h"
#include "Contacts/ContactsManger.h"
#include "UI/UI.h"
#include "QueueMessage/QueueMessageAcceptMessage.h"
#include "QueueMessage/QueueMessageDeliveryMessage.h"
#include "QueueMessage/QueueMessageKeyboardCommand.h"
#include "QueueMessage/QueueMessagePingDone.h"
#include "QueueMessage/QueueMessageTypingChar.h"
#include "QueueMessage/QueueMessageShowPage.h"
#include "QueueMessage/QueueMessageTypingMessage.h"
#include "QueueMessage/QueueMessageShowProperties.h"
#include "QueueMessage/QueueMessageEditProperty.h"
#include <Arduino.h>
#include <stdexcept>

RadioChat::RadioChat()
    : settings_(nullptr)
    , esp_(nullptr)
    , keyHandler_(nullptr)
    , display_(nullptr)
    , radio_(nullptr)
    , ledIndicator_(nullptr)
    , sound_(nullptr)
    , battery_(nullptr)
    , ui_(nullptr)
    , contactsManager_(nullptr)
{
}

RadioChat::~RadioChat() = default;

void RadioChat::init()
{
    using namespace std::placeholders;

#if DEBUG_MODE == 1
    // in debug always print start info
    // for it need serial
    // else init serial by logger settings
    Logger::instance().initSerialLogging();
#endif
    {
        FLASH.init();
        FLASH.mkdir(STORAGE_DIR);
    }

    settings_ = new Settings();
    settings_->init(STORAGE_DIR "/" SETTINGS_FILENAME);
    
    {
        Logger::instance().init(*settings_);
        FLASH.printInfo(); // need logger for print
    }
    {
        esp_ = new Esp();
        esp_->init(*settings_);
    }
    {
        radio_ = new Radio();
        radio_->init(*settings_,
            std::bind(&RadioChat::pushAcceptMessage, this, _1, _2, _3),
            std::bind(&RadioChat::pushDeliveryMessage, this, _1, _2),
            std::bind(&RadioChat::pushPingDone, this, _1, _2));
    }
    {
        ledIndicator_ = new LedIndicator();
        ledIndicator_->init(*settings_);
    }
    {
        sound_ = new Sound();
        sound_->init(*settings_);
    }
    {
        battery_ = new Battery();
        battery_->init(*settings_);
    }
    {
        contactsManager_ = new ContactsManager();
        contactsManager_->init(*settings_);
    }
    {
        keyHandler_ = new KeyHandler();
        keyHandler_->init(*settings_,
            std::bind(&RadioChat::pushTypingChar, this, _1),
            std::bind(&RadioChat::pushKeyboardCommand, this, _1));
    }
    {
        display_ = new Display();
        display_->init(*settings_);
    }
    {
        ui_ = new UI();
        UIContext  uiContext(display_, settings_, battery_, radio_, contactsManager_, 0, 0, 0,
            std::bind(&RadioChat::pushShowPage, this, _1),
            std::bind(&RadioChat::pushShowPageTypingMessage, this, _1),
            std::bind(&RadioChat::pushShowPagePropertyList, this, _1),
            std::bind(&RadioChat::pushShowPageChooseOption, this, _1)
        );
        ui_->init(uiContext);
    }
    if (sound_->getSettings().enablePlayOnPowerOn) {
        sound_->play(Melody::Name::Nokia);
    }
    LOG_INF("Firmware version: %04X", FIRMWARE_VERSION);
    runThreadCheckQueue();
}

void RadioChat::loop()
{
    radio_->check();
    ledIndicator_->check();
    battery_->check();
    ui_->draw();
}


void RadioChat::runThreadCheckQueue()
{
    // use raw function for create thread 
    // because in std::thread stack size is small
    xTaskCreatePinnedToCore(this->checkQueueThread, "QueueCheck", 10 * 1024, this, (configMAX_PRIORITIES - 1) / 2, NULL, ARDUINO_RUNNING_CORE);
}

void RadioChat::checkQueueThread(void* thisPtr)
{
    RadioChat* self = static_cast<RadioChat*>(thisPtr);
    while (true) {
        self->checkQueue();
    }
}

void RadioChat::checkQueue()
{
    auto msg = messageQueue_.dequeue();
    
    //UBaseType_t freeStack = uxTaskGetStackHighWaterMark(NULL);
    //LOG_DBG("RadioChat::checkQueue Free stack: %u", freeStack);

    switch (msg->getType())
    {
    case QueueMessageType::AcceptMessage: {
        auto m = static_cast<QueueMessageAcceptMessage*>(msg.get());
        LOG_INF("Accept message %u from %u: %s", m->getID(), m->getAddress(), m->getMessage().c_str());
        sound_->play(Melody::Name::PackmanShort);
        ui_->showIncomingMessage(m->getMessage(), m->getAddress());
        break;
    }
    case QueueMessageType::DeliveryMessage: {
        auto m = static_cast<QueueMessageDeliveryMessage*>(msg.get());
        LOG_INF("Delivered message %u to %u", m->getID(), m->getAddress());
        sound_->play(Melody::Name::Accept);
        break;
    }   
    case QueueMessageType::KeyboardCommand: {
        auto m = static_cast<QueueMessageKeyboardCommand*>(msg.get());
        ui_->onKeyCommand(m->getCommand());
        break;
    }
    case QueueMessageType::PingDone: {
        //auto m = static_cast<QueueMessagePingDone*>(msg.get());
        break;
    }
    case QueueMessageType::TypingChar: {
        auto m = static_cast<QueueMessageTypingChar*>(msg.get());
        ui_->onChar(m->getCode());
        break;
    }
    case QueueMessageType::ShowPage: {
        auto m = static_cast<QueueMessageShowPage*>(msg.get());
        ui_->setCurrentPage(m->getPageType());
        break;
    }
    case QueueMessageType::TypingMessage: {
        auto m = static_cast<QueueMessageTypingMessage*>(msg.get());
        ui_->showTypingMessage(m->getAddress());
        break;
    }
    case QueueMessageType::ShowProperties: {
        auto m = static_cast<QueueMessageShowProperties*>(msg.get());
        ui_->showPropertyList(m->getProperties());
    break;
    }
    case QueueMessageType::EditProperty: {
        auto m = static_cast<QueueMessageEditProperty*>(msg.get());
        ui_->showEditProperty(m->getProperty());
        break;
    }
    default:
        break;
    }
}

void RadioChat::pushTypingChar(uint16_t code)
{
    auto msg = std::make_unique<QueueMessageTypingChar>(code);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushKeyboardCommand(KeyCommand cmd)
{
    auto msg = std::make_unique<QueueMessageKeyboardCommand>(cmd);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushAcceptMessage(uint16_t sender, uint8_t msgID, const std::string& text)
{
    auto msg = std::make_unique<QueueMessageAcceptMessage>(sender, msgID , text);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushDeliveryMessage(uint16_t address, uint8_t msgID)
{
    auto msg = std::make_unique<QueueMessageDeliveryMessage>(address, msgID);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushPingDone(uint16_t address, uint32_t delay)
{
    auto msg = std::make_unique<QueueMessagePingDone>(address, delay);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushShowPage(UIPageType pageType)
{
    auto msg = std::make_unique<QueueMessageShowPage>(pageType);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushShowPageTypingMessage(uint16_t address)
{
    auto msg = std::make_unique<QueueMessageTypingMessage>(address);
    messageQueue_.enqueue(std::move(msg));
}


void RadioChat::pushShowPagePropertyList(const PropertyMap& properties)
{
    auto msg = std::make_unique<QueueMessageShowProperties>(properties);
    messageQueue_.enqueue(std::move(msg));
}

void RadioChat::pushShowPageChooseOption(const Property& prop)
{
    auto msg = std::make_unique<QueueMessageEditProperty>(prop);
    messageQueue_.enqueue(std::move(msg));
}
