#include "UIPageChatSelect.h"
#include "../Contacts/ContactsManger.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"

UIPageChatSelect::UIPageChatSelect(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::ChatSelect, parent, context)
{
    using namespace std::placeholders;

    addItemSimple("Общий", std::bind(&UIPageChatSelect::onSharedChatClick, this));
    addItemSimple("Новый контакт", std::bind(&UIPageChatSelect::onNewContactClick, this));
    addItemSimple("-----------------------");

    const auto& contacts = ctx_->contactsManager->getContacts();
    for (const Contact& contact : contacts) {
        addItem(ItemType::Number, contact.name, std::to_string(contact.address), std::bind(&UIPageChatSelect::onContactClick, this));
    }
}

UIPageChatSelect::~UIPageChatSelect() = default;

void UIPageChatSelect::onSharedChatClick()
{
    ctx_->showPageTypingMessage(BROADCAST_ADDRESS);
}

void UIPageChatSelect::onNewContactClick()
{
}

void UIPageChatSelect::onContactClick()
{
}
