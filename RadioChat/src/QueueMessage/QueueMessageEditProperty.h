#pragma once 

#include "QueueMessageBase.h"
#include "../Settings/Property.h"
#include <stdint.h>

class QueueMessageEditProperty : public QueueMessageBase
{
public:
    QueueMessageEditProperty(const Property& prop);
    ~QueueMessageEditProperty();
    const Property& getProperty() const;

private:
    Property property_;
};