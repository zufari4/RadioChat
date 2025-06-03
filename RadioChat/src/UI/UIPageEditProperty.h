#pragma once

#include "BaseMenu.h"
#include "../Settings/Property.h"
#include <mutex>

class UIPageEditProperty : public BaseMenu
{
public:
    UIPageEditProperty(UIPageType parent, const UIContext* context);
    ~UIPageEditProperty();
    void setProperty(const Property& property);
    void draw() override;
    void onChar(uint16_t symbol) override;
    void onKeyCommand(KeyCommand cmd) override;

protected:
    void onItemClick(uint8_t itemIndex) override;
private:
    void saveProperty();

    Property property_;
    std::string currentValue_;
    char carriageChar_;
    std::mutex mtxValue_;
    bool isEditable_;
};