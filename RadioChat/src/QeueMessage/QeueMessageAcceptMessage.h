#pragma once 

#include "QeueMessageBase.h"
#include <stdint.h>
#include <string>

class QeueMessageAcceptMessage: public QeueMessageBase
{
public:
    QeueMessageAcceptMessage(const std::string& msg, uint8_t id);
    ~QeueMessageAcceptMessage();
    std::string getMessage() const;
    uint8_t getID() const;

private:
    std::string msg_;
    uint8_t id_;
};
