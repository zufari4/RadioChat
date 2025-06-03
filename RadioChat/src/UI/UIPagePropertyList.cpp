#include "UIPagePropertyList.h"
#include "../Settings/Settings.h"
#include "../Logger/Logger.h"

UIPagePropertyList::UIPagePropertyList(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::PropertyList, parent, context)
{
}

UIPagePropertyList::~UIPagePropertyList() = default;

void UIPagePropertyList::onItemClick(uint8_t itemIndex)
{
    if (itemIndex < properties_.size()) {
        auto it = properties_.begin();
        std::advance(it, itemIndex);
        const auto& property = it->second;
        ctx_->showPageEditProperty(property);
    }
}

void UIPagePropertyList::setProperties(const PropertyMap& properties)
{
    properties_ = properties;
    for (const auto& property : properties_) {
        switch (property.second.valueType) {
        case ValueType::String:
            addItem(ItemType::String, property.second.caption, property.second.value);
            break;
        case ValueType::Int:
            addItem(ItemType::Number, property.second.caption, property.second.value);
            break;
        case ValueType::Bool:
            addItem(ItemType::Bool, property.second.caption, getCaptionValue(property.second));
            break;
        case ValueType::Enum:
            addItem(ItemType::StringList, property.second.caption, getCaptionValue(property.second));
            break;
        case ValueType::Float:
            addItem(ItemType::Real, property.second.caption, getCaptionValue(property.second));
            break;
        default:
            LOG_ERR("Unknown property value type %d for %s", static_cast<int>(property.second.valueType), property.second.name.c_str());
            break;
        }
    }
}

