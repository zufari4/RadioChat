#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessageShowContactActions : public QueueMessageBase
{
public:
    QueueMessageShowContactActions(uint16_t address);
    ~QueueMessageShowContactActions();
    uint16_t getAddress() const;

private:
    uint16_t address_;
};