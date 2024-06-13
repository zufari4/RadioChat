#include "QeueMessageTypingChar.h"

QeueMessageTypingChar::QeueMessageTypingChar(uint16_t code)
    : QeueMessageBase(QeueMessageType::TypingChar)
    , code_(code)
{
}

QeueMessageTypingChar::~QeueMessageTypingChar()
{
}

uint16_t QeueMessageTypingChar::getCode() const
{
    return code_;
}