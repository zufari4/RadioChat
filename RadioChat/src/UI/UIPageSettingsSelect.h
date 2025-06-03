#pragma once

#include "BaseMenu.h"

class UIPageSettingsSelect : public BaseMenu
{
public:
    UIPageSettingsSelect(UIPageType parent, const UIContext* context);
    ~UIPageSettingsSelect();

protected:
    void onItemClick(uint8_t itemIndex) override;
};