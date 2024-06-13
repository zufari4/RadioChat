#include "QeueMessageDeliveryMessage.h"

QeueMessageDeliveryMessage::QeueMessageDeliveryMessage(uint16_t address, uint16_t msgID)
    : QeueMessageBase(QeueMessageType::DeliveryMessage)
    , address_(address)
    , id_(msgID)
{
}

QeueMessageDeliveryMessage::~QeueMessageDeliveryMessage()
{
}

uint16_t QeueMessageDeliveryMessage::getAddress() const
{
    return address_;
}

uint16_t QeueMessageDeliveryMessage::getID() const
{
    return id_;
}