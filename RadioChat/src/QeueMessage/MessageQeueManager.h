#pragma once

#include "QeueMessageBase.h"
#include "SafeQeue.h"
#include <memory>
#include <inttypes.h>

enum class KeyCommand;

class MessageQeueManager
{
public:
    using MessagePtr = std::unique_ptr<QeueMessageBase>;

    MessageQeueManager();
    ~MessageQeueManager();
    MessagePtr pop();
    void pushTypingChar(uint16_t code);
    void pushKeyboardCommand(KeyCommand cmd);
    void pushAcceptMessage(const std::string& text, uint8_t msgID);
    void pushDeliveryMessage(uint16_t address, uint8_t msgID);
    void pushPingDone(uint16_t address, uint32_t delay);
    
private:
    SafeQueue<MessagePtr> qeue_;
};