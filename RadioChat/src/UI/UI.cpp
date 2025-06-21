#include "UI.h"
#include "UIPageMain.h"
#include "UIPageIncomingMessage.h"
#include "UIPageChatSelect.h"
#include "UIPageChat.h"
#include "UIPageTypingMessage.h"
#include "UIPageSettingsSelect.h"
#include "UIPagePropertyList.h"
#include "UIPageEditProperty.h"
#include "UIPageContacts.h"
#include "UIPageContactActions.h"
#include "UIPagePing.h"
#include "../Logger/Logger.h"
#include "../Display/Display.h"
#include "../Settings/Settings.h"
#include "../Radio/Lora.h"
#include "../Chat/ChatManager.h"

UI::UI() = default;
UI::~UI() = default;

void UI::init(const UIContext& context)
{
    LOG_INF("-- Initialize UI --");
    ctx_ = context;
    ctx_.uiSettings = loadSettings(*ctx_.settings);
    ctx_.maxLineChars = ctx_.display->getDisplayWidth()/ctx_.display->getMaxCharWidth();
    ctx_.textHeight = ctx_.uiSettings.textHeight;
    ctx_.maxCountLines = ctx_.display->getDisplayHeight()/ctx_.textHeight;
    setCurrentPage(UIPageType::Main);
}

void UI::draw()
{
    std::lock_guard guard(pageMutex_);
    ctx_.display->clear();
    currentPage_->draw();
    ctx_.display->flush();
}

void UI::onChar(uint16_t symbol)
{
    std::lock_guard guard(pageMutex_);
    currentPage_->onChar(symbol);
}

void UI::onKeyCommand(KeyCommand cmd)
{
    std::lock_guard guard(pageMutex_);
    currentPage_->onKeyCommand(cmd);
}

void UI::onIncomingMessage(const std::string &message, uint16_t senderAddress, uint16_t destAddress)
{
     std::lock_guard guard(pageMutex_);
     if (currentPage_->getType() == UIPageType::Chat) {
         auto chatPage = static_cast<UIPageChat*>(currentPage_.get());
         chatPage->onIncomingMessage(message, senderAddress, destAddress);
     }
}

void UI::showTypingMessage(UIPageType parent, TypingMessageAction action, uint16_t destinationAddress)
{
    auto page = createPage(UIPageType::TypingMessage);
    if (page) {
        auto pageTypingMessage = static_cast<UIPageTypingMessage*>(page.get());
        pageTypingMessage->setParent(parent);
        pageTypingMessage->setAddress(destinationAddress);
        pageTypingMessage->setAction(action);
        setCurrentPage(std::move(page));
    }
}

void UI::showPropertyList(const PropertyMap& properties)
{
    auto page = createPage(UIPageType::PropertyList);
    if (page) {
        auto pagePropertyList = static_cast<UIPagePropertyList*>(page.get());
        pagePropertyList->setProperties(properties);
        setCurrentPage(std::move(page));
    }
}

void UI::showEditProperty(const Property& prop)
{
    auto page = createPage(UIPageType::EditProperty);
    if (page) {
        auto pageEditProperty = static_cast<UIPageEditProperty*>(page.get());
        pageEditProperty->setProperty(prop);
        setCurrentPage(std::move(page));
    }
}

void UI::showChatContact(uint16_t address)
{
    auto page = createPage(UIPageType::Chat);
    if (page) {
        auto pageChatContact = static_cast<UIPageChat*>(page.get());
        pageChatContact->init(address);
        setCurrentPage(std::move(page));
    }
}

void UI::showContactActions(uint16_t address)
{
    auto page = createPage(UIPageType::ContactActions);
    if (page) {
        auto pageChatContact = static_cast<UIPageContactActions*>(page.get());
        pageChatContact->setAddress(address);
        setCurrentPage(std::move(page));
    }
}

void UI::showPing(uint16_t address)
{
    auto page = createPage(UIPageType::Ping);
    if (page) {
        auto pageMain = static_cast<UIPagePing*>(page.get());
        pageMain->setAddress(address);
        setCurrentPage(std::move(page));
    }
}

void UI::setCurrentPage(UIPageType pageType)
{
    std::unique_ptr<UIPageBase> page = createPage(pageType);
    if (page) {
        setCurrentPage(std::move(page));
    }
}

void UI::setCurrentPage(std::unique_ptr<UIPageBase> page)
{
    std::lock_guard guard(pageMutex_);
    currentPage_ = std::move(page);
}

std::unique_ptr<UIPageBase> UI::createPage(UIPageType pageType)
{
    std::unique_ptr<UIPageBase> newPage;

    switch (pageType)
    {
    case UIPageType::Main:
        newPage = std::make_unique<UIPageMain>(&ctx_);
        break;
    case UIPageType::IncomingMessage:
        newPage = std::make_unique<UIPageIncomingMessage>(UIPageType::Main, &ctx_);
        break;
    case UIPageType::ChatSelect:
        newPage = std::make_unique<UIPageChatSelect>(UIPageType::Main, &ctx_);
        break;
    case UIPageType::Chat:
        newPage = std::make_unique<UIPageChat>(UIPageType::ChatSelect, &ctx_);
        break;
    case UIPageType::TypingMessage:
        newPage = std::make_unique<UIPageTypingMessage>(UIPageType::ChatSelect, &ctx_);
        break;
    case UIPageType::SettingsSelect:
        newPage = std::make_unique<UIPageSettingsSelect>(UIPageType::Main, &ctx_);
        break;
    case UIPageType::PropertyList:
        newPage = std::make_unique<UIPagePropertyList>(UIPageType::SettingsSelect, &ctx_);
        break;
    case UIPageType::EditProperty:
        newPage = std::make_unique<UIPageEditProperty>(UIPageType::SettingsSelect, &ctx_);
        break;
    case UIPageType::ContactActions:
        newPage = std::make_unique<UIPageContactActions>(UIPageType::Main, &ctx_);
        break;
    case UIPageType::Contacts:
        newPage = std::make_unique<UIPageContacts>(UIPageType::Main, &ctx_);
        break;
    case UIPageType::Ping:
        newPage = std::make_unique<UIPagePing>(UIPageType::Main, &ctx_);
        break;
    default:
        LOG_ERR("Unknown page type %u", static_cast<uint8_t>(pageType));
    }

    return newPage;
}

UISettings UI::loadSettings(Settings& settings) const
{
    UISettings uiSettings;
    auto props = settings.ui();
    uiSettings.carriageChar = Settings::get_s(eUITextCarriageChar, props)[0];
    uiSettings.textHeight = Settings::get_i(eUITextHeight, props);
    uiSettings.carriageShowTime = Settings::get_i(eUITextCarriageShowTime, props);
    return uiSettings;
}

