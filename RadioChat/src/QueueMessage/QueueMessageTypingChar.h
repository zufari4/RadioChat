#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>

class QueueMessageTypingChar: public QueueMessageBase
{
public:
    QueueMessageTypingChar(uint16_t code);
    ~QueueMessageTypingChar();
    uint16_t getCode() const;

private:
    uint16_t code_;
};
