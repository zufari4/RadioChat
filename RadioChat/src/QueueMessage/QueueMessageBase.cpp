#include "QueueMessageBase.h"

QueueMessageBase::QueueMessageBase(QueueMessageType type)
    : type_(type)
{
}

QueueMessageBase::~QueueMessageBase()
{
}

QueueMessageType QueueMessageBase::getType() const
{
    return type_;
}
