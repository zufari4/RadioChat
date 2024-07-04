#include "UIPageChat.h"
#include "../Contacts/ContactsManger.h"
#include "../Logger/Logger.h"

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

void UIPageChat::onKeyCommand(KeyCommand cmd)
{
    std::lock_guard guard(pageMtx_);

    if (currentPage_) {
        currentPage_->onKeyCommand(cmd);
        if (currentPage_->getExitStatus() == ExitStatus::Cancel || currentPage_->getExitStatus() == ExitStatus::Accept) {
            currentPage_.reset();
        }
        return;
    }
    if (cmd == KeyCommand::Escape) {
        setExitStatus(ExitStatus::Cancel);
        return;
    }
    BaseMenu::onKeyCommand(cmd);
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

void UIPageChat::onSharedChatClick(Item& item)
{
}

void UIPageChat::onNewContactClick(Item& item)
{
}

void UIPageChat::onContactClick(Item& item)
{
}
