#include "QueueMessageTypingChar.h"

QueueMessageTypingChar::QueueMessageTypingChar(uint16_t code)
    : QueueMessageBase(QueueMessageType::TypingChar)
    , code_(code)
{
}

QueueMessageTypingChar::~QueueMessageTypingChar()
{
}

uint16_t QueueMessageTypingChar::getCode() const
{
    return code_;
}