#pragma once 

#include "QueueMessageBase.h"
#include "../Settings/Property.h"
#include <stdint.h>

class QueueMessageChooseOption : public QueueMessageBase
{
public:
    QueueMessageChooseOption(const Property& prop);
    ~QueueMessageChooseOption();
    const Property& getProperty() const;

private:
    Property property_;
};