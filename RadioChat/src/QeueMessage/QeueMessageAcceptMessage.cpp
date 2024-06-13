#include "QeueMessageAcceptMessage.h"

QeueMessageAcceptMessage::QeueMessageAcceptMessage(const std::string& msg, uint8_t id)
    : QeueMessageBase(QeueMessageType::AcceptMessage)
    , msg_(msg)
    , id_(id)
{
}

QeueMessageAcceptMessage::~QeueMessageAcceptMessage()
{
}

std::string QeueMessageAcceptMessage::getMessage() const
{
    return msg_;
}

uint8_t QeueMessageAcceptMessage::getID() const
{
    return id_;
}