#pragma once

#include <string>
#include <map>
#include "PropertyType.h"
#include "ValueType.h"
#include "EnumOption.h"
#include "PropertySection.h"

struct Property
{
    PropertyType type = PropertyType::eNone;
    PropertySection section = PropertySection::None;
    std::string caption;
    std::string name;
    std::string value;
    ValueType valueType = ValueType::None;
    EnumOption enumOption;
};

using PropertyMap = std::map<PropertyType, Property>;