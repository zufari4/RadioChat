#include "UIPageBase.h"

UIPageBase::UIPageBase(UIPageType type, const UIContext* contex)
    : ctx_(contex)
    , type_(type)
{
}

UIPageBase::~UIPageBase()
{
}

void UIPageBase::draw() {}
void UIPageBase::onChar(uint16_t symbol) {}
void UIPageBase::onKeyCommand(KeyCommand cmd) {}

UIPageBase::ExitStatus UIPageBase::getExitStatus() const
{
    return exitStatus_;
}

void UIPageBase::setExitStatus(ExitStatus status)
{
    exitStatus_ = status;
}
