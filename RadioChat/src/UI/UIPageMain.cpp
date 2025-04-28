#include "UIPageMain.h"
#include "UIPageChatSelect.h"
#include "../Battery/Battery.h"
#include "../Logger/Logger.h"
#include <cstring>

UIPageMain::UIPageMain(const UIContext* context)
    : BaseMenu(UIPageType::Main, UIPageType::None, context)
{
    addItem(ItemType::Real, "Батарея");
    addItemSimple("Чат");
    addItemSimple("Контакты");
    addItemSimple("Настройки");
    addItemSimple("Журнал");
    addItemSimple("Заметки");
    addItemSimple("Перезагрузить");
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

void UIPageMain::onItemClick(uint8_t itemIndex)
{
    LOG_DBG("UIPageMain::onItemClick %u", itemIndex);
    switch (itemIndex) {
    case 0: // Battery
        break;
    case 1: // Chat
        ctx_->setCurrentPage(UIPageType::ChatSelect);
        break;
    case 2: // Contacts
        ctx_->setCurrentPage(UIPageType::Contacts);
        break;
    case 3: // Settings
        ctx_->setCurrentPage(UIPageType::Settings);
        break;
    case 4: // Logs
        ctx_->setCurrentPage(UIPageType::Logs);
        break;
    case 5: // Notes
        break;
    case 6: // Reboot
        break;
    default:
        break;
    }
}
