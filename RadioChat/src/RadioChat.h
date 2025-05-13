#pragma once

#include <stdint.h>
#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include "UI/UIPageType.h"
#include "QueueMessage/SafeQeue.h"

class Flash;
class Settings;
class Esp;
class KeyHandler;
class Display;
class Radio;
class LedIndicator;
class UI;
class Sound;
class Battery;
class ContactsManager;
class QueueMessageBase;
enum class KeyCommand;


class RadioChat
{
public:
    RadioChat();
    ~RadioChat();
    static void run();

private:
    using MessagePtr = std::unique_ptr<QueueMessageBase>;
    static RadioChat& instance();
    static void svc(void* thisPtr);
    static void checkQueueThread(void* thisPtr);
    void runThreadCheckQueue();
    void runThreadSvc();
    void checkQueue();
    void init();
    void loop();
    void pushTypingChar(uint16_t code);
    void pushKeyboardCommand(KeyCommand cmd);
    void pushAcceptMessage(uint16_t sender, uint8_t msgID, const std::string& text);
    void pushDeliveryMessage(uint16_t address, uint8_t msgID);
    void pushPingDone(uint16_t address, uint32_t delay);
    void pushShowPage(UIPageType pageType);
    void pushShowPageTypingMessage(uint16_t address);

    Settings*     settings_;
    Esp*          esp_;
    KeyHandler*   keyHandler_;
    Display*      display_;
    Radio*        radio_;
    LedIndicator* ledIndicator_;
    Sound*        sound_;
    Battery*      battery_;
    UI*           ui_;
    ContactsManager* contactsManager_;
    SafeQueue<MessagePtr> messageQueue_;
    std::thread   svcThread_;
};