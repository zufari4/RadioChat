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

void BaseMenu::addItem(ItemType type, std::string_view caption, std::string_view value)
{
    items_.emplace_back(type, caption, value);
}

void BaseMenu::addItemSimple(std::string_view caption)
{
    items_.emplace_back(ItemType::String, caption, "");
}

void BaseMenu::setItemValue(uint8_t index, std::string_view value)
{
    if (index < items_.size()) {
        items_[index].value = value;
    }
}

void BaseMenu::drawText(uint8_t x, uint8_t y, uint8_t width, std::string_view str, bool invert)
{
    if (invert) {
        ctx_->display->drawButtonWith(x, y, width, str);
    }
    else {
        ctx_->display->drawStr(x, y, str);
    }
}
void BaseMenu::drawItem(uint8_t y, const Item& item, bool invert)
{
    uint16_t dsipW = ctx_->display->getDisplayWidth();
    uint8_t width = item.value.empty() ? dsipW : dsipW - ctx_->display->getStrWidth(item.value);
    drawText(0, y, width, item.caption, invert);

    if (!item.value.empty()) {
        uint8_t x = dsipW - ctx_->display->getStrWidth(item.value);
        drawText(x, y, dsipW, item.value, invert);
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

std::string_view BaseMenu::bool_str(bool val)
{
    return val ? "да" : "нет";
}

bool BaseMenu::str_bool(std::string_view str)
{
    return str == "да";
}

void BaseMenu::selectItem(uint8_t index)
{
      selected_ = index;
      if (selected_ >= ctx_->maxCountLines) {
          offset_ = selected_ - ctx_->maxCountLines + 1;
      }
      else {
          offset_ = 0;
      }
}

const std::string& BaseMenu::getItemValue(uint8_t index) const
{
    if (index < items_.size()) {
        return items_[index].value;
    }
    static const std::string emptyStr;
    return emptyStr;
}
