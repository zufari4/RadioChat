#pragma once

#include "QueueMessageType.h"

class QueueMessageBase
{
public:
    QueueMessageBase(QueueMessageType type);
    ~QueueMessageBase();
    QueueMessageType getType() const;

private:
    QueueMessageType type_;
};