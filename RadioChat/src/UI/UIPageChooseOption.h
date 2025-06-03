#pragma once

#include "BaseMenu.h"
#include "../Settings/Property.h"
#include <vector>


class UIPageChooseOption : public BaseMenu
{
public:
    UIPageChooseOption(UIPageType parent, const UIContext* context);
    ~UIPageChooseOption();
    void setProperty(const Property& prop);

protected:
    void onItemClick(uint8_t itemIndex) override;

private:
    Property property_;
};