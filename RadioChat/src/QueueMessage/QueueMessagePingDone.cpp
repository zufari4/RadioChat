#include "QueueMessagePingDone.h"

QueueMessagePingDone::QueueMessagePingDone(uint16_t address, uint32_t delay)
    : QueueMessageBase(QueueMessageType::PingDone)
    , address_(address)
    , delay_(delay)
{
}

QueueMessagePingDone::~QueueMessagePingDone()
{
}

uint16_t QueueMessagePingDone::getAddress() const
{
    return address_;
}

uint16_t QueueMessagePingDone::getDelay() const
{
    return delay_;
}