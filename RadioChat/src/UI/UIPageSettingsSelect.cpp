#include "UIPageSettingsSelect.h"
#include "UIContext.h"
#include "../Settings/Settings.h"
#include "../Chat/ChatManager.h"

UIPageSettingsSelect::UIPageSettingsSelect(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::SettingsSelect, parent, context)
{
    addItemSimple("Логирование");
    addItemSimple("ESP");
    addItemSimple("Клавиатура");
    addItemSimple("Дисплей");
    addItemSimple("Интерфейс");
    addItemSimple("Радио");
    addItemSimple("Светодиоды");
    addItemSimple("Звук");
    addItemSimple("Аккумулятор");
    addItemSimple("Контакты");
    addItemSimple("Сброс настроек");
    addItemSimple("Очистить общий чат");
}

UIPageSettingsSelect::~UIPageSettingsSelect() = default;

void UIPageSettingsSelect::onItemClick(uint8_t itemIndex)
{
    if (itemIndex == 0) {
        ctx_->showPagePropertyList(ctx_->settings->logger());
    }
    else if (itemIndex == 1) {
        ctx_->showPagePropertyList(ctx_->settings->esp());
    }
    else if (itemIndex == 2) {
        ctx_->showPagePropertyList(ctx_->settings->keyboard());
    }
    else if (itemIndex == 3) {
        ctx_->showPagePropertyList(ctx_->settings->display());
    }
    else if (itemIndex == 4) {
        ctx_->showPagePropertyList(ctx_->settings->ui());
    }
    else if (itemIndex == 5) {
        ctx_->showPagePropertyList(ctx_->settings->radio());
    }
    else if (itemIndex == 6) {
        ctx_->showPagePropertyList(ctx_->settings->led());
    }
    else if (itemIndex == 7) {
        ctx_->showPagePropertyList(ctx_->settings->sound());
    }
    else if (itemIndex == 8) {
        ctx_->showPagePropertyList(ctx_->settings->battery());
    }
    else if (itemIndex == 9) {
        ctx_->showPagePropertyList(ctx_->settings->contacts());
    }
    else if (itemIndex == 10) {
        ctx_->settings->saveDefaultSettings();
        ctx_->setCurrentPage(parent_);
    }
    else if (itemIndex == 11) {
        ctx_->chatManager->clearSharedChat();
        ctx_->setCurrentPage(parent_);
    }
}
