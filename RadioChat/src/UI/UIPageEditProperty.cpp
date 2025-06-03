#include "UIPageEditProperty.h"
#include "../Settings/Settings.h"
#include "../Logger/Logger.h"
#include "../Display/Display.h"
#include "../Utils.h"

UIPageEditProperty::UIPageEditProperty(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::EditProperty, parent, context)
    , carriageChar_(context->uiSettings.carriageChar)
    , isEditable_(false)
{
}

UIPageEditProperty::~UIPageEditProperty() = default;

void UIPageEditProperty::onItemClick(uint8_t itemIndex)
{
    bool isChanged = false;

    if (property_.valueType == ValueType::Enum) {
        if (itemIndex < property_.enumOption.size()) {
            const auto& option = property_.enumOption[itemIndex];
            if (option.value != property_.value) {
                isChanged = true;
                property_.value = option.value;
            }
        }
    } else if (property_.valueType == ValueType::Bool) {
        std::string newValue = itemIndex == 0 ? "1" : "0";
        if (newValue != property_.value) {
            isChanged = true;
            property_.value = newValue;
        }
    }
    if (isChanged) {
        saveProperty();
    }
}

void UIPageEditProperty::setProperty(const Property& property)
{
    property_ = property;
    switch (property_.valueType) {
    case ValueType::String:
    case ValueType::Int:
    case ValueType::Float: {
        currentValue_ = property.value;
        isEditable_ = true;
        break;
    }
    case ValueType::Enum: {
        uint8_t itemIndex = 0;
        for (uint8_t i = 0; i < property_.enumOption.size(); ++i) {
            const auto& option = property_.enumOption[i];
            addItemSimple(option.caption);
            if (option.value == property_.value) {
                itemIndex = i;
            }
        }
        selectItem(itemIndex);
        break;
    }
    case ValueType::Bool:
        addItemSimple("on");
        addItemSimple("off");
        if (property_.value == "1") {
            selectItem(0);
        } else {
            selectItem(1);
        }
        break;
    default:
        LOG_ERR("Unknown property value type %d for %s", static_cast<int>(property_.valueType), property_.name.c_str());
        break;
    }
}

void UIPageEditProperty::draw()
{
    if (isEditable_) {
        std::lock_guard guard(mtxValue_);
        ctx_->display->drawStr(0, 0, currentValue_ + carriageChar_);
    }
    else {
        BaseMenu::draw();
    }
}

void UIPageEditProperty::onChar(uint16_t symbol)
{
    if (isEditable_) {
        std::lock_guard guard(mtxValue_);
        currentValue_ += utils::to_str(symbol);
    }
}

void UIPageEditProperty::onKeyCommand(KeyCommand cmd)
{
    if (isEditable_) {
        std::lock_guard guard(mtxValue_);
        switch (cmd) {
        case KeyCommand::Backspace:
            if (!currentValue_.empty()) {
                currentValue_.pop_back();
            }
            break;
        case KeyCommand::Enter:
            if (currentValue_ != property_.value) {
                property_.value = currentValue_;
                saveProperty();
            }
            break;
        default:
            BaseMenu::onKeyCommand(cmd);
            break;
        }
    } else {
        BaseMenu::onKeyCommand(cmd);
    }
}

void UIPageEditProperty::saveProperty()
{
    if (ctx_->settings->setProperty(property_)) {
        LOG_INF("Property %s saved successfully", property_.name.c_str());
        ctx_->setCurrentPage(parent_);
    } else {
        LOG_ERR("Failed to save property %s", property_.name.c_str());
    }
}

