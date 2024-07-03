#include "UIPageMain.h"
#include "../Battery/Battery.h"
#include "../Logger/Logger.h"
#include <cstring>

UIPageMain::UIPageMain(const UIContext* context)
    : BaseMenu(UIPageType::Main, context)
{
    addItem(ItemType::Real  , "Батарея", "0.0", nullptr);
    addItem(ItemType::String, "Чат", "", std::bind(&UIPageMain::chatClick, this));
    addItem(ItemType::String, "Настройки", "", std::bind(&UIPageMain::settingsClick, this));
    addItem(ItemType::String, "Журнал", "", std::bind(&UIPageMain::logsClick, this));
    addItem(ItemType::String, "Заметки", "", std::bind(&UIPageMain::notesClick, this));
    addItem(ItemType::String, "Перезагрузить", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "Салам", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "Попалам", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "Трампапапа", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "фысфысфыс", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "цупацуп", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "ваиваиваиваи", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "прленлгенлен", "", std::bind(&UIPageMain::rebootClick, this));
    addItem(ItemType::String, "ывываываыва", "", std::bind(&UIPageMain::rebootClick, this));
}

UIPageMain::~UIPageMain()
{
}

void UIPageMain::draw()
{
    float vBatt = ctx_->battery->getVoltage();
    if (vBatt != prevVBatt_) {
        prevVBatt_ = vBatt;
        snprintf(currentVBatt_, sizeof(currentVBatt_), "%.1f", vBatt);
        if (std::strcmp(currentVBatt_, prevVBattStr_.c_str()) != 0) {
            prevVBattStr_ = currentVBatt_;
            setItemValue(0, prevVBattStr_);
        };
    }

    BaseMenu::draw();
}

void UIPageMain::chatClick()
{
}

void UIPageMain::settingsClick()
{
}

void UIPageMain::rebootClick()
{
}

void UIPageMain::logsClick()
{
}

void UIPageMain::notesClick()
{
}
