#include "QueueMessageTypingMessage.h"

QueueMessageTypingMessage::QueueMessageTypingMessage(uint16_t destinationAddress)
    : QueueMessageBase(QueueMessageType::TypingMessage)
    , destinationAddress_(destinationAddress)
{
}

QueueMessageTypingMessage::~QueueMessageTypingMessage() = default;

uint16_t QueueMessageTypingMessage::getAddress() const
{
    return destinationAddress_;
}

