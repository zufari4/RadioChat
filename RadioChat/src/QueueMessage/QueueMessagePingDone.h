#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessagePingDone: public QueueMessageBase
{
public:
    QueueMessagePingDone(uint16_t address, uint32_t delay);
    ~QueueMessagePingDone();
    uint16_t getAddress() const;
    uint16_t getDelay() const;

private:
    uint16_t address_;
    uint32_t delay_;
};