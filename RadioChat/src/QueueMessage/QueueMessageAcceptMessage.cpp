#include "QueueMessageAcceptMessage.h"

QueueMessageAcceptMessage::QueueMessageAcceptMessage(uint16_t sender, uint16_t dest, uint8_t msgID, const std::string& text)
    : QueueMessageBase(QueueMessageType::AcceptMessage)
    , senderAddress_(sender)
    , destAddress_(dest)
    , id_(msgID)
    , msg_(text)
{
}

QueueMessageAcceptMessage::~QueueMessageAcceptMessage()
{
}

uint16_t QueueMessageAcceptMessage::getSenderAddress() const
{
    return senderAddress_;
}

uint16_t QueueMessageAcceptMessage::getDestAddress() const
{
    return destAddress_;
}

std::string QueueMessageAcceptMessage::getMessage() const
{
    return msg_;
}

uint8_t QueueMessageAcceptMessage::getID() const
{
    return id_;
}