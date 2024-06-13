#pragma once 

#include "QeueMessageBase.h"
#include <stdint.h>

class QeueMessageDeliveryMessage: public QeueMessageBase
{
public:
    QeueMessageDeliveryMessage(uint16_t address, uint16_t msgID);
    ~QeueMessageDeliveryMessage();
    uint16_t getAddress() const;
    uint16_t getID() const;

private:
    uint16_t address_;
    uint16_t id_;
};