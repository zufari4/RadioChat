#pragma once

#include "BaseMenu.h"

class UIPageChatSelect: public BaseMenu
{
public:
    UIPageChatSelect(UIPageType parent, const UIContext* context);
    ~UIPageChatSelect();

private:
    void onSharedChatClick();
    void onNewContactClick();
    void onContactClick();
};