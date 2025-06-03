#include "UIPagePropertyList.h"
#include "../Settings/Settings.h"

UIPagePropertyList::UIPagePropertyList(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::PropertyList, parent, context)
{
}

UIPagePropertyList::~UIPagePropertyList() = default;

void UIPagePropertyList::onItemClick(uint8_t itemIndex)
{
}

void UIPagePropertyList::setProperties(const PropertyMap& properties)
{
    properties_ = properties;
    for (const auto& property : properties_) {
        addItemSimple(property.second.caption);
    }
}

