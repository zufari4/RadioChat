#include "UIPageBase.h"
#include "../Display/Display.h"
#include "../Logger/Logger.h"
#include "../Configuration.h"

UIPageBase::UIPageBase(UIPageType type, const UISettings& settings, Display* display)
    : type_(type)
    , settings_(settings)
    , display_(display)
    , maxStrLen_(display->getDisplayWidth()/display->getMaxCharWidth())
    , textHeight_(settings.textHeight)
    , maxCountLines_(display->getDisplayHeight()/textHeight_)
{
    LOG_DBG("maxStrLen %u textHeight %u maxCountLines %u", maxStrLen_, textHeight_, maxCountLines_);
}

UIPageBase::~UIPageBase()
{
}

void UIPageBase::draw() {}
void UIPageBase::onChar(uint16_t symbol) {}
void UIPageBase::onKeyCommand(KeyCommand cmd) {}

void UIPageBase::drawText(uint8_t x, uint8_t y, const std::string& str)
{
    display_->drawStr(x, y, str);
}

uint8_t UIPageBase::getMaxStrLen() const
{
    return maxStrLen_;
}

uint8_t UIPageBase::getTextHeight() const
{
    return textHeight_;
}

uint8_t UIPageBase::getMaxCountLines() const
{
    return maxCountLines_;
}

uint8_t UIPageBase::getTextWidth(const std::string& str) const
{
    return display_->getStrWidth(str.c_str());
}