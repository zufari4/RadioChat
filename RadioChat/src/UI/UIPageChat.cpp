#include "UIPageChat.h"
#include "../Contacts/ContactsManger.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"

UIPageChat::UIPageChat(const UIContext* context)
    : BaseMenu(UIPageType::Chat, context)
{
    LOG_DBG("Chat page");

    using namespace std::placeholders;

    addItemSimple("Общий", std::bind(&UIPageChat::onSharedChatClick, this, _1));
    addItemSimple("Новый контакт", std::bind(&UIPageChat::onNewContactClick, this, _1));
    addItemSimple("-----------------------");

    const auto& contacts = ctx_->contactsManager->getContacts();
    for (const Contact& contact : contacts) {
        addItem(ItemType::Number, contact.name, std::to_string(contact.address), std::bind(&UIPageChat::onContactClick, this, _1));
    }
}

UIPageChat::~UIPageChat()
{
}

void UIPageChat::onChar(uint16_t symbol) 
{
    std::lock_guard guard(pageMtx_);

    if (currentPage_) {
        currentPage_->onChar(symbol);
    }
    else {
        BaseMenu::onChar(symbol);
    }
}

void UIPageChat::onKeyCommand(KeyCommand cmd)
{
    std::lock_guard guard(pageMtx_);

    if (currentPage_) {
        currentPage_->onKeyCommand(cmd);
        if (currentPage_->getExitStatus() == ExitStatus::Cancel || currentPage_->getExitStatus() == ExitStatus::Accept) {
            currentPage_.reset();
        }
    }
    else if (cmd == KeyCommand::Escape) {
        setExitStatus(ExitStatus::Cancel);
        return;
    }
    else {
        BaseMenu::onKeyCommand(cmd);
    }
}

void UIPageChat::draw()
{
    std::lock_guard guard(pageMtx_);

    if (currentPage_) {
        currentPage_->draw();
    }
    else {
        BaseMenu::draw();
    }
}

void UIPageChat::onSharedChatClick(Item* item)
{
    std::lock_guard guard(pageMtx_);
    currentPage_ = std::make_unique<UIPageTypingMessage>(ctx_, BROADCAST_ADDRESS);
}

void UIPageChat::onNewContactClick(Item* item)
{
}

void UIPageChat::onContactClick(Item* item)
{
}
