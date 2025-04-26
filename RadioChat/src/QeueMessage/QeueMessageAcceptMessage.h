#pragma once 

#include "QeueMessageBase.h"
#include <stdint.h>
#include <string>

class QeueMessageAcceptMessage: public QeueMessageBase
{
public:
    QeueMessageAcceptMessage(uint16_t sender, uint8_t msgID, const std::string& text);
    ~QeueMessageAcceptMessage();
    uint16_t getAddress() const;
    std::string getMessage() const;
    uint8_t getID() const;

private:
    uint16_t address_;
    uint8_t id_;
    std::string msg_;
};
