#pragma once

#include "UIPageBase.h"
#include <functional>
#include <vector>
#include <atomic>

class BaseMenu: public UIPageBase
{
public:
    using ClickCallback = std::function<void()>;

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
        ClickCallback onClick;
        Item(ItemType _type, const std::string& _caption, const std::string& _value, ClickCallback _onClick)
            : type(_type), caption(_caption), value(_value), onClick(_onClick) {}
    };

    BaseMenu(UIPageType type, UIPageType parent, const UIContext* contex);
    ~BaseMenu();
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;

protected:
    void addItem(ItemType type, const std::string& caption, const std::string& value = "", ClickCallback onClick = nullptr);    
    void addItemSimple(const std::string& caption, ClickCallback onClick = nullptr);
    void setItemValue(uint8_t index, const std::string& value);

private:
    void drawItem(uint8_t y, const Item& item, bool invert);

    std::vector<Item> items_;
    std::atomic_uint8_t offset_;
    std::atomic_uint8_t selected_;
};