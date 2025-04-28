#include "BaseMenu.h"
#include "../Display/Display.h"
#include "../Logger/Logger.h"

BaseMenu::BaseMenu(UIPageType type, UIPageType parent, const UIContext* context)
    : UIPageBase(type, parent, context)
    , offset_(0)
    , selected_(0)
{
}

BaseMenu::~BaseMenu() = default;

void BaseMenu::addItem(ItemType type, const std::string& caption, const std::string& value)
{
    items_.emplace_back(type, caption, value);
}

void BaseMenu::addItemSimple(const std::string &caption)
{
    items_.emplace_back(ItemType::String, caption, "");
}

void BaseMenu::setItemValue(uint8_t index, const std::string &value)
{
    if (index < items_.size()) {
        items_[index].value = value;
    }
}

void BaseMenu::drawItem(uint8_t y, const Item &item, bool invert)
{
    std::string line = item.caption;
    if (!item.value.empty()) {
        line += ":\t" + item.value;
    }
    if (invert) {
        ctx_->display->drawButtonFullWith(0, y, line);
    }
    else {
        ctx_->display->drawStr(0, y, line);
    }
}

void BaseMenu::draw()
{
    uint8_t stopLine = std::min((uint8_t)items_.size(), (uint8_t)(offset_ + ctx_->maxCountLines));
    uint8_t y = 0;
    
    for (uint8_t line = offset_; line < stopLine; ++line) {
        drawItem(y, items_[line], line == selected_);
        y += ctx_->textHeight;
    }
}

void BaseMenu::onKeyCommand(KeyCommand cmd)
{
    if (cmd == KeyCommand::Enter) {
        if (selected_ < items_.size()) {
            uint8_t selected = selected_.load();
            LOG_DBG("selected item %u", selected);
            onItemClick(selected);
        }
    }
    else if (cmd == KeyCommand::Up) {
        if (items_.size() >= 2 && selected_ > 0) {
            selected_ -= 1;
            if (offset_ > selected_ && offset_ > 0) {
                offset_--;
            }
        }
    }
    else if (cmd == KeyCommand::Down) {
        if (items_.size() >= 2 && ((selected_ < (items_.size() - 1))))
            selected_ += 1;
        if ((offset_ + ctx_->maxCountLines) <= selected_) {
            offset_++;
        }
    }
    else {
        UIPageBase::onKeyCommand(cmd);
    }
}

void BaseMenu::onItemClick(uint8_t itemIndex)
{
}
