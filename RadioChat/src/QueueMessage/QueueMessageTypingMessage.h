#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessageTypingMessage : public QueueMessageBase
{
public:
    QueueMessageTypingMessage(uint16_t destinationAddress);
    ~QueueMessageTypingMessage();
    uint16_t getAddress() const;

private:
    uint16_t destinationAddress_;
};