#include "QueueMessageDeliveryMessage.h"

QueueMessageDeliveryMessage::QueueMessageDeliveryMessage(uint16_t address, uint16_t msgID)
    : QueueMessageBase(QueueMessageType::DeliveryMessage)
    , address_(address)
    , id_(msgID)
{
}

QueueMessageDeliveryMessage::~QueueMessageDeliveryMessage()
{
}

uint16_t QueueMessageDeliveryMessage::getAddress() const
{
    return address_;
}

uint16_t QueueMessageDeliveryMessage::getID() const
{
    return id_;
}