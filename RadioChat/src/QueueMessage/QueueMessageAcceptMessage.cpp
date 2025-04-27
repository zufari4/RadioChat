#include "QueueMessageAcceptMessage.h"

QueueMessageAcceptMessage::QueueMessageAcceptMessage(uint16_t sender, uint8_t msgID, const std::string& text)
    : QueueMessageBase(QueueMessageType::AcceptMessage)
    , address_(sender)
    , id_(msgID)
    , msg_(text)
{
}

QueueMessageAcceptMessage::~QueueMessageAcceptMessage()
{
}

uint16_t QueueMessageAcceptMessage::getAddress() const
{
    return address_;
}

std::string QueueMessageAcceptMessage::getMessage() const
{
    return msg_;
}

uint8_t QueueMessageAcceptMessage::getID() const
{
    return id_;
}