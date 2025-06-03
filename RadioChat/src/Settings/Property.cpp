#include "Property.h"
#include <iomanip>
#include <sstream>

std::string getCaptionValue(const Property& property)
{
    if (property.valueType == ValueType::Enum) {
        for (const auto& option : property.enumOption) {
            if (option.value == property.value) {
                return option.caption;
            }
        }
    }
    else if (property.valueType == ValueType::Bool) {
        return property.value == "1" ? "on" : "off";
    }
    else if (property.valueType == ValueType::Float) {
        float floatValue = std::stof(property.value);
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << floatValue;
        return stream.str();
    }

    return property.value;
}
