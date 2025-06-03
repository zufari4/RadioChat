#pragma once 

#include "QueueMessageBase.h"
#include "../Settings/Property.h"
#include <stdint.h>

class QueueMessageShowProperties : public QueueMessageBase
{
public:
    QueueMessageShowProperties(const PropertyMap& properties);
    ~QueueMessageShowProperties();
    const PropertyMap& getProperties() const;

private:
    PropertyMap properties_;
};