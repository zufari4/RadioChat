#include "QueueMessageEditProperty.h"

QueueMessageEditProperty::QueueMessageEditProperty(const Property& prop)
    : QueueMessageBase(QueueMessageType::EditProperty)
    , property_(prop)
{
}

QueueMessageEditProperty::~QueueMessageEditProperty() = default;

const Property& QueueMessageEditProperty::getProperty() const
{
    return property_;
}

