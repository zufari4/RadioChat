#include "UIPageBase.h"
#include "../Logger/Logger.h"

UIPageBase::UIPageBase(UIPageType type, UIPageType parent, const UIContext* context)
    : type_(type)
    , parent_(parent)
    , ctx_(context)
{
    LOG_DBG("Create page %s parent %s", uiPageTypeToStr(type), uiPageTypeToStr(parent));
}

UIPageBase::~UIPageBase() = default;
void UIPageBase::draw() {}
void UIPageBase::onChar(uint16_t symbol) {}

void UIPageBase::onKeyCommand(KeyCommand cmd)
{
    if (cmd == KeyCommand::Escape) {
        showParentPage();
    }
}

UIPageType UIPageBase::getType() const
{
    return type_;
}

void UIPageBase::setParent(UIPageType parent)
{
    parent_ = parent;
}

void UIPageBase::showParentPage()
{
    if (parent_ != UIPageType::None) {
        LOG_DBG("%s Return to %s", __FUNCTION__, uiPageTypeToStr(parent_));
        ctx_->setCurrentPage(parent_);
    }
}
