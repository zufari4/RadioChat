#include "UIPageContacts.h"
#include "../Contacts/ContactsManger.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"

UIPageContacts::UIPageContacts(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::Contacts, parent, context)
{
    addItemSimple("Добавить");

    const auto& contacts = ctx_->contactsManager->getContacts();
    for (const Contact& contact : contacts) {
        addItem(ItemType::Number, contact.name, std::to_string(contact.address));
    }
}

UIPageContacts::~UIPageContacts() = default;

void UIPageContacts::onItemClick(uint8_t itemIndex)
{
    if (itemIndex == 0) {
        ctx_->showPageTypingMessage(UIPageType::Contacts, TypingMessageAction::AddContact, 0);
    }
    else {
        ctx_->showPageContactActions(std::stoul(getItemValue(itemIndex)));
    }
}
