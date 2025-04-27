#include "UIPageMain.h"
#include "UIPageChatSelect.h"
#include "../Battery/Battery.h"
#include "../Logger/Logger.h"
#include <cstring>

UIPageMain::UIPageMain(const UIContext* context)
    : BaseMenu(UIPageType::Main, UIPageType::None, context)
{
    addItem(ItemType::Real, "Батарея");
    addItemSimple("Чат", std::bind(&UIPageMain::chatClick, this));
    addItemSimple("Контакты", std::bind(&UIPageMain::contactsClick, this));
    addItemSimple("Настройки", std::bind(&UIPageMain::settingsClick, this));
    addItemSimple("Журнал", std::bind(&UIPageMain::logsClick, this));
    addItemSimple("Заметки", std::bind(&UIPageMain::notesClick, this));
    addItemSimple("Перезагрузить", std::bind(&UIPageMain::rebootClick, this));
}

UIPageMain::~UIPageMain() = default;

void UIPageMain::draw()
{
    updateBatteryVoltage();
    BaseMenu::draw();
}

void UIPageMain::updateBatteryVoltage()
{
    float vBatt = ctx_->battery->getVoltage();
    if (vBatt != prevVBatt_) {
        prevVBatt_ = vBatt;
        snprintf(currentVBatt_, sizeof(currentVBatt_), "%.1fv", vBatt);
        if (std::strcmp(currentVBatt_, prevVBattStr_.c_str()) != 0) {
            prevVBattStr_ = currentVBatt_;
            setItemValue(0, prevVBattStr_);
        };
    }
}

void UIPageMain::chatClick()
{
    ctx_->setCurrentPage(UIPageType::ChatSelect);
}

void UIPageMain::contactsClick()
{
    ctx_->setCurrentPage(UIPageType::Contacts);
}

void UIPageMain::settingsClick()
{
    ctx_->setCurrentPage(UIPageType::Settings);
}

void UIPageMain::rebootClick()
{
}

void UIPageMain::logsClick()
{
    ctx_->setCurrentPage(UIPageType::Logs);
}

void UIPageMain::notesClick()
{
}
