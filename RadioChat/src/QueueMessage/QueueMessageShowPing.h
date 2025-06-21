#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessageShowPing : public QueueMessageBase
{
public:
    QueueMessageShowPing(uint16_t address);
    ~QueueMessageShowPing();
    uint16_t getAddress() const;

private:
    uint16_t address_;
};