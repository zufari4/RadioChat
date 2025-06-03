#pragma once

#include "UIPageBase.h"
#include <vector>
#include <atomic>
#include <string_view>

class BaseMenu: public UIPageBase
{
public:
    enum class ItemType
    {
        String,
        Number,
        Bool,
        Real,
        StringList,
    };

    struct Item
    {
        ItemType type = ItemType::String;
        std::string caption;
        std::string value;

        Item(ItemType _type, std::string_view _caption, std::string_view _value)
            : type(_type), caption(_caption), value(_value) {}
    };

    BaseMenu(UIPageType type, UIPageType parent, const UIContext* contex);
    ~BaseMenu();
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;

protected:
    void addItem(ItemType type, std::string_view caption, std::string_view value);
    void addItemSimple(std::string_view caption);
    void setItemValue(uint8_t index, std::string_view value);
    virtual void onItemClick(uint8_t itemIndex);
    static std::string_view bool_str(bool val);
    static bool str_bool(std::string_view str);

private:
    void drawItem(uint8_t y, const Item& item, bool invert);
    void drawText(uint8_t x, uint8_t y, uint8_t width, std::string_view str, bool invert);

    std::vector<Item> items_;
    std::atomic_uint8_t offset_;
    std::atomic_uint8_t selected_;
};