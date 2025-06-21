#include "QueueMessageShowContactActions.h"

QueueMessageShowContactActions::QueueMessageShowContactActions(uint16_t address)
    : QueueMessageBase(QueueMessageType::ShowContactActions)
    , address_(address)
{
}

QueueMessageShowContactActions::~QueueMessageShowContactActions() = default;

uint16_t QueueMessageShowContactActions::getAddress() const
{
    return address_;
}

