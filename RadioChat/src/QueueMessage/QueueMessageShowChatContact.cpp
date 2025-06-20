#include "QueueMessageShowChatContact.h"

QueueMessageShowChatContact::QueueMessageShowChatContact(uint16_t address)
    : QueueMessageBase(QueueMessageType::ShowChatContact)
    , address_(address)
{
}

QueueMessageShowChatContact::~QueueMessageShowChatContact() = default;

uint16_t QueueMessageShowChatContact::getAddress() const
{
    return address_;
}

