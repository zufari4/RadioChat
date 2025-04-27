#include "UIPageBase.h"
#include "../Logger/Logger.h"

UIPageBase::UIPageBase(UIPageType type, UIPageType parent, const UIContext* context)
    : type_(type)
    , parent_(parent)
    , ctx_(context)
{
    LOG_DBG("Show page %s parent %s", uiPageTypeToStr(type), uiPageTypeToStr(parent));
}

UIPageBase::~UIPageBase() = default;

void UIPageBase::draw() {}
void UIPageBase::onChar(uint16_t symbol) {}

void UIPageBase::onKeyCommand(KeyCommand cmd)
{
    if (cmd == KeyCommand::Escape) {
        if (parent_ != UIPageType::None) {
            ctx_->setCurrentPage(parent_);
        }
    }
}

UIPageType UIPageBase::getType() const
{
    return type_;
}
