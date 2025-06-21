#pragma once

#include "BaseMenu.h"

class UIPagePing: public BaseMenu
{
public:
    UIPagePing(UIPageType parent, const UIContext* context);
    ~UIPagePing();
    void setAddress(uint16_t address);

protected:
    uint16_t address_; // Address of the contact for actions
};