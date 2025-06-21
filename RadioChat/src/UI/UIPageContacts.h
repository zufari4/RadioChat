#pragma once

#include "BaseMenu.h"

class UIPageContacts: public BaseMenu
{
public:
    UIPageContacts(UIPageType parent, const UIContext* context);
    ~UIPageContacts();

protected:
    void onItemClick(uint8_t itemIndex) override;
};