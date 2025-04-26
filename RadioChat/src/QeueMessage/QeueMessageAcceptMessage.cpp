#include "QeueMessageAcceptMessage.h"

QeueMessageAcceptMessage::QeueMessageAcceptMessage(uint16_t sender, uint8_t msgID, const std::string& text)
    : QeueMessageBase(QeueMessageType::AcceptMessage)
    , address_(sender)
    , id_(msgID)
    , msg_(text)
{
}

QeueMessageAcceptMessage::~QeueMessageAcceptMessage()
{
}

uint16_t QeueMessageAcceptMessage::getAddress() const
{
    return address_;
}

std::string QeueMessageAcceptMessage::getMessage() const
{
    return msg_;
}

uint8_t QeueMessageAcceptMessage::getID() const
{
    return id_;
}