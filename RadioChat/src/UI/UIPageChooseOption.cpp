#include "UIPageChooseOption.h"
#include "UIContext.h"
#include "../Settings/Settings.h"

UIPageChooseOption::UIPageChooseOption(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::ChooseOption, parent, context)
{
}

UIPageChooseOption::~UIPageChooseOption() = default;

void UIPageChooseOption::onItemClick(uint8_t itemIndex)
{
    property_.value = property_.enumOption[itemIndex].value;
    ctx_->settings->setProperty(property_);
    showParentPage();
}

void UIPageChooseOption::setProperty(const Property& prop)
{
    property_ = prop;
    for (const auto& opt : property_.enumOption) {
        addItemSimple(opt.caption);
    }
}
