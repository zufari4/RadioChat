#pragma once 

#include "QueueMessageBase.h"
#include <stdint.h>
#include <string>

class QueueMessageAcceptMessage: public QueueMessageBase
{
public:
    QueueMessageAcceptMessage(uint16_t sender, uint8_t msgID, const std::string& text);
    ~QueueMessageAcceptMessage();
    uint16_t getAddress() const;
    std::string getMessage() const;
    uint8_t getID() const;

private:
    uint16_t address_;
    uint8_t id_;
    std::string msg_;
};
