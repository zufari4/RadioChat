#include "QueueMessageTypingMessage.h"

QueueMessageTypingMessage::QueueMessageTypingMessage(UIPageType parent, TypingMessageAction action, uint16_t destinationAddress)
    : QueueMessageBase(QueueMessageType::TypingMessage)
    , parent_(parent)
    , action_(action)
    , destinationAddress_(destinationAddress)
{
}

QueueMessageTypingMessage::~QueueMessageTypingMessage() = default;

uint16_t QueueMessageTypingMessage::getAddress() const
{
    return destinationAddress_;
}

UIPageType QueueMessageTypingMessage::getParentPage() const
{
    return parent_;
}

TypingMessageAction QueueMessageTypingMessage::getAction() const
{
    return action_;
}

