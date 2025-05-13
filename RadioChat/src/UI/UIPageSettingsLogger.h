#pragma once

#include "BaseMenu.h"

class UIPageChatSelect : public BaseMenu
{
public:
    UIPageChatSelect(UIPageType parent, const UIContext* context);
    ~UIPageChatSelect();

protected:
    void onItemClick(uint8_t itemIndex) override;
};