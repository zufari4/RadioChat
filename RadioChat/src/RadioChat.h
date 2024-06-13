#pragma once

#include <stdint.h>
#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include "QeueMessage/SafeQeue.h"

class Flash;
class Settings;
class Esp;
class KeyHandler;
class Display;
class Radio;
class LedIndicator;
class UI;
class Sound;
class QeueMessageBase;
enum class KeyCommand;


class RadioChat
{
public:
    RadioChat();
    ~RadioChat();
    void init();
    void loop();

private:
    using MessagePtr = std::unique_ptr<QeueMessageBase>;

    void svc();
    void pushTypingChar(uint16_t code);
    void pushKeyboardCommand(KeyCommand cmd);
    void pushAcceptMessage(const std::string& text, uint8_t msgID);
    void pushDeliveryMessage(uint16_t address, uint8_t msgID);
    void pushPingDone(uint16_t address, uint32_t delay);

    std::atomic_bool workFlag_;
    Settings*     settings_;
    Flash*        flash_;
    Esp*          esp_;
    KeyHandler*   keyHandler_;
    Display*      display_;
    Radio*        radio_;
    UI*           ui_;
    LedIndicator* ledIndicator_;
    Sound*        sound_;
    SafeQueue<MessagePtr> messageQeue_;
    std::thread   svcThread_;
};