#include "UIPageMain.h"
#include "UIPageChat.h"
#include "../Battery/Battery.h"
#include "../Logger/Logger.h"
#include <cstring>

UIPageMain::UIPageMain(const UIContext* context)
    : BaseMenu(UIPageType::Main, context)
{
    using namespace std::placeholders;
    LOG_DBG("Main page");
    addItem(ItemType::Real, "Батарея");
    addItemSimple("Чат", std::bind(&UIPageMain::chatClick, this, _1));
    addItemSimple("Контакты", std::bind(&UIPageMain::chatClick, this, _1));
    addItemSimple("Настройки", std::bind(&UIPageMain::settingsClick, this, _1));
    addItemSimple("Журнал", std::bind(&UIPageMain::logsClick, this, _1));
    addItemSimple("Заметки", std::bind(&UIPageMain::notesClick, this, _1));
    addItemSimple("Перезагрузить", std::bind(&UIPageMain::rebootClick, this, _1));
}

UIPageMain::~UIPageMain()
{
}

void UIPageMain::draw()
{
    std::lock_guard guard(pageMtx_);

    if (subPage_) {
        subPage_->draw();
    }
    else {
        drawMainMenu();
    }
}

void UIPageMain::onKeyCommand(KeyCommand cmd)
{
    std::lock_guard guard(pageMtx_);

    if (subPage_) {
        subPage_->onKeyCommand(cmd);
        if (subPage_->getExitStatus() == ExitStatus::Cancel || subPage_->getExitStatus() == ExitStatus::Accept) {
            LOG_DBG("Show main menu");
            subPage_.reset();
        }
    }
    else {
        BaseMenu::onKeyCommand(cmd);
    }
}

void UIPageMain::drawMainMenu()
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

    BaseMenu::draw();
}

void UIPageMain::chatClick(Item& item)
{
    std::lock_guard guard(pageMtx_);
    subPage_ = std::make_unique<UIPageChat>(ctx_);
}

void UIPageMain::settingsClick(Item& item)
{
}

void UIPageMain::rebootClick(Item& item)
{
}

void UIPageMain::logsClick(Item& item)
{
}

void UIPageMain::notesClick(Item& item)
{
}
