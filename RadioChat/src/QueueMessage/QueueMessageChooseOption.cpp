#include "QueueMessageChooseOption.h"

QueueMessageChooseOption::QueueMessageChooseOption(const Property& prop)
    : QueueMessageBase(QueueMessageType::ChooseOption)
    , property_(prop)
{
}

QueueMessageChooseOption::~QueueMessageChooseOption() = default;

const Property& QueueMessageChooseOption::getProperty() const
{
    return property_;
}

