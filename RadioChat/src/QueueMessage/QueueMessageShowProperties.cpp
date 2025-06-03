#include "QueueMessageShowProperties.h"

QueueMessageShowProperties::QueueMessageShowProperties(const PropertyMap& properties)
    : QueueMessageBase(QueueMessageType::ShowProperties)
    , properties_(properties)
{
}
QueueMessageShowProperties::~QueueMessageShowProperties() = default;

const PropertyMap& QueueMessageShowProperties::getProperties() const
{
    return properties_;
}



