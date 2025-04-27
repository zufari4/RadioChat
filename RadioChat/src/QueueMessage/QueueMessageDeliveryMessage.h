#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessageDeliveryMessage: public QueueMessageBase
{
public:
    QueueMessageDeliveryMessage(uint16_t address, uint16_t msgID);
    ~QueueMessageDeliveryMessage();
    uint16_t getAddress() const;
    uint16_t getID() const;

private:
    uint16_t address_;
    uint16_t id_;
};