#include "UIPageChatSelect.h"
#include "../Contacts/ContactsManger.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"

UIPageChatSelect::UIPageChatSelect(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::ChatSelect, parent, context)
{
    addItemSimple("Общий");
    addItemSimple("-----------------------");

    const auto& contacts = ctx_->contactsManager->getContacts();
    for (const Contact& contact : contacts) {
        addItem(ItemType::Number, contact.name, std::to_string(contact.address));
    }
}

UIPageChatSelect::~UIPageChatSelect() = default;

void UIPageChatSelect::onItemClick(uint8_t itemIndex)
{
    if (itemIndex == 0) { // Shared chat
        ctx_->showPageChatContact(BROADCAST_ADDRESS);
    }
    else if (itemIndex > 1) { // Contact
        uint16_t address = std::stoul(getItemValue(itemIndex));
        if (ctx_->contactsManager->getContact(address).address == 0) {
            LOG_ERR("Contact with address %u not found", address);
            return;
        }
        LOG_DBG("Selected contact with address %u", address);
        ctx_->showPageChatContact(address);
    }
}
