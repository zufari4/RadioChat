#pragma once

#include "BaseMenu.h"

class UIPageContactActions: public BaseMenu
{
public:
    UIPageContactActions(UIPageType parent, const UIContext* context);
    ~UIPageContactActions();
    void setAddress(uint16_t address);

protected:
    void onItemClick(uint8_t itemIndex) override;
    uint16_t address_; // Address of the contact for actions
};