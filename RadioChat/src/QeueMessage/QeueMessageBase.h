#pragma once

#include "QeueMessageType.h"

class QeueMessageBase
{
public:
    QeueMessageBase(QeueMessageType type);
    ~QeueMessageBase();
    QeueMessageType getType() const;

private:
    QeueMessageType type_;
};