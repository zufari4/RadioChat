#pragma once

#include "BaseMenu.h"
#include "../Settings/Property.h"

class UIPagePropertyList : public BaseMenu
{
public:
    UIPagePropertyList(UIPageType parent, const UIContext* context);
    ~UIPagePropertyList();
    void setProperties(const PropertyMap& properties);

protected:
    void onItemClick(uint8_t itemIndex) override;
private:
    PropertyMap properties_;
};