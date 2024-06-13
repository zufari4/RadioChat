#pragma once 

#include "QeueMessageBase.h"
#include <stdint.h>

class QeueMessageTypingChar: public QeueMessageBase
{
public:
    QeueMessageTypingChar(uint16_t code);
    ~QeueMessageTypingChar();
    uint16_t getCode() const;

private:
    uint16_t code_;
};
