#pragma once 

#include "QeueMessageBase.h"
#include <stdint.h>

class QeueMessagePingDone: public QeueMessageBase
{
public:
    QeueMessagePingDone(uint16_t address, uint32_t delay);
    ~QeueMessagePingDone();
    uint16_t getAddress() const;
    uint16_t getDelay() const;

private:
    uint16_t address_;
    uint32_t delay_;
};