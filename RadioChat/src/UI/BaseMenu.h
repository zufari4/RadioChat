#pragma once

#include "UIPageBase.h"
#include <vector>
#include <atomic>

class BaseMenu: public UIPageBase
{
public:
    enum class ItemType
    {
        String,
        Number,
        Bool,
        Real
    };

    struct Item
    {
        ItemType type = ItemType::String;
        std::string caption;
        std::string value;

        Item(ItemType _type, const std::string& _caption, const std::string& _value)
            : type(_type), caption(_caption), value(_value) {}
    };

    BaseMenu(UIPageType type, UIPageType parent, const UIContext* contex);
    ~BaseMenu();
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;

protected:
    void addItem(ItemType type, const std::string& caption, const std::string& value = "");
    void addItemSimple(const std::string& caption);
    void setItemValue(uint8_t index, const std::string& value);
    virtual void onItemClick(uint8_t itemIndex);

private:
    void drawItem(uint8_t y, const Item& item, bool invert);

    std::vector<Item> items_;
    std::atomic_uint8_t offset_;
    std::atomic_uint8_t selected_;
};