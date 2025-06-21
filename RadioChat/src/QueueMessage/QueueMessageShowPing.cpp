#include "QueueMessageShowPing.h"

QueueMessageShowPing::QueueMessageShowPing(uint16_t address)
    : QueueMessageBase(QueueMessageType::ShowContactActions)
    , address_(address)
{
}

QueueMessageShowPing::~QueueMessageShowPing() = default;

uint16_t QueueMessageShowPing::getAddress() const
{
    return address_;
}

