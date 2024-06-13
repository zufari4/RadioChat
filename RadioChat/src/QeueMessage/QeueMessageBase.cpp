#include "QeueMessageBase.h"

QeueMessageBase::QeueMessageBase(QeueMessageType type)
    : type_(type)
{
}

QeueMessageBase::~QeueMessageBase()
{
}

QeueMessageType QeueMessageBase::getType() const
{
    return type_;
}
