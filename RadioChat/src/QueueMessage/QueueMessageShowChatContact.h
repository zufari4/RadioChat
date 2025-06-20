#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessageShowChatContact : public QueueMessageBase
{
public:
    QueueMessageShowChatContact(uint16_t address);
    ~QueueMessageShowChatContact();
    uint16_t getAddress() const;

private:
    uint16_t address_;
};