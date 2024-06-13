#include "QeueMessagePingDone.h"

QeueMessagePingDone::QeueMessagePingDone(uint16_t address, uint32_t delay)
    : QeueMessageBase(QeueMessageType::PingDone)
    , address_(address)
    , delay_(delay)
{
}

QeueMessagePingDone::~QeueMessagePingDone()
{
}

uint16_t QeueMessagePingDone::getAddress() const
{
    return address_;
}

uint16_t QeueMessagePingDone::getDelay() const
{
    return delay_;
}