#include "UI.h"
#include "UIPageMain.h"
#include "UIPageIncomingMessage.h"
#include "UIPageChatSelect.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Display/Display.h"

UI::UI() = default;
UI::~UI() = default;

void UI::init(const UIContext& context)
{
    LOG_INF("-- Initialize UI --");

    ctx_ = context;
    ctx_.maxStrLen = ctx_.display->getDisplayWidth()/ctx_.display->getMaxCharWidth();
    ctx_.textHeight = ctx_.uiSettings.textHeight;
    ctx_.maxCountLines = ctx_.display->getDisplayHeight()/ctx_.textHeight;
    LOG_DBG("maxStrLen %u textHeight %u maxCountLines %u", ctx_.maxStrLen, ctx_.textHeight, ctx_.maxCountLines);
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

void UI::showIncomingMessage(const std::string &message, uint16_t address)
{
    auto page = createPage(UIPageType::IncomingMessage);
    if (page) {
        auto pageIncomingMessage = static_cast<UIPageIncomingMessage*>(page.get());
        pageIncomingMessage->setMessage(message, address);
        setCurrentPage(std::move(page));
    }
}

void UI::showTypingMessage(uint16_t destinationAddress)
{
    auto page = createPage(UIPageType::TypingMessage);
    if (page) {
        auto pageTypingMessage = static_cast<UIPageTypingMessage*>(page.get());
        pageTypingMessage->setAddress(destinationAddress);
        setCurrentPage(std::move(page));
    }
}

void UI::setCurrentPage(UIPageType pageType)
{
    auto page = createPage(pageType);
    if (page) {
        setCurrentPage(std::move(page));
    }
}

void UI::setCurrentPage(std::unique_ptr<UIPageBase> pageType)
{
    std::lock_guard guard(pageMutex_);
    currentPage_ = std::move(pageType);
}

std::unique_ptr<UIPageBase> UI::createPage(UIPageType pageType)
{
    UIPageType parent = currentPage_ != nullptr ? currentPage_->getType() : UIPageType::None;
    std::unique_ptr<UIPageBase> newPage;

    switch (pageType)
    {
    case UIPageType::Main:
        newPage = std::make_unique<UIPageMain>(&ctx_);
        break;
    case UIPageType::IncomingMessage:
        newPage = std::make_unique<UIPageIncomingMessage>(parent, &ctx_);
        break;
    case UIPageType::ChatSelect:
        newPage = std::make_unique<UIPageChatSelect>(parent, &ctx_);
        break;
    case UIPageType::TypingMessage:
        newPage = std::make_unique<UIPageTypingMessage>(parent, &ctx_);
        break;
    default:
        LOG_ERR("Unknown page type %u", static_cast<uint8_t>(pageType));
    }

    return newPage;
}


