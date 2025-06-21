#include "UIPageContactActions.h"
#include "../Contacts/ContactsManger.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"

UIPageContactActions::UIPageContactActions(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::ContactActions, parent, context)
    , address_(0)
{
    addItemSimple("Отправить сообщение");
    addItemSimple("Пинг");
    addItemSimple("Удалить");
    addItemSimple("Изменить");
}

UIPageContactActions::~UIPageContactActions() = default;

void UIPageContactActions::setAddress(uint16_t address)
{
    address_ = address;
}

void UIPageContactActions::onItemClick(uint8_t itemIndex)
{
    if (itemIndex == 0) {
        ctx_->showPageTypingMessage(UIPageType::Chat, TypingMessageAction::SendMessage, address_);
    } else if (itemIndex == 1) {
        ctx_->showPagePing(address_);
    } else if (itemIndex == 2) {
        ctx_->contactsManager->removeContact(address_);
        ctx_->setCurrentPage(UIPageType::Contacts);
    } else if (itemIndex == 3) {
        ctx_->showPageTypingMessage(UIPageType::Contacts, TypingMessageAction::EditContact, address_);
    }
}
