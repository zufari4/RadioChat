#include "UIPageChat.h"
#include "../Chat/ChatManager.h"
#include "../Contacts/ContactsManger.h"
#include "../Display/Display.h"
#include "../Radio/Radio.h"
#include "../Utils.h"
#include "../Logger/Logger.h"
#include <algorithm>

UIPageChat::UIPageChat(UIPageType parent, const UIContext* context)
    : UIPageBase(UIPageType::Chat, parent, context)
    , lastMsgIndex_(0)
    , lineOffset_(0)
    , canLoadPrevMsg_(true)
    , canLoadNextMsg_(true)
    , countMessagesPerPage_(context->maxCountLines * 3) // Load 3 times the max count of lines to ensure we have smothing to scroll through
    , destAddress_(BROADCAST_ADDRESS)
    , selfAddress_(ctx_->radio->getSettings().selfAddress)
{
}

UIPageChat::~UIPageChat() = default;

void UIPageChat::draw()
{
    std::lock_guard g(mutexMsg_);

    if (messages_.totalCountLines == 0) {
        ctx_->display->drawStr(0, 0, "Нет сообщений");
        return;
    }

    const uint8_t dispH = ctx_->display->getDisplayHeight();
    uint8_t offsetY = 0;
    uint16_t lineIndex = 0;

    for (const auto& msg : messages_.list) {
        for (const std::string& line : msg) {
            if (lineIndex < lineOffset_) {
                lineIndex++;
                continue; // Skip lines until we reach the offset
            }
            ctx_->display->drawStr(0, offsetY, line.c_str());
            offsetY += ctx_->textHeight;
            lineIndex++;

            if (offsetY >= dispH) {
                // If we reach the end of the display, stop drawing
                return;
            }
        }
    }
}

void UIPageChat::onKeyCommand(KeyCommand cmd)
{
    if (cmd == KeyCommand::Enter) {
        ctx_->showPageTypingMessage(UIPageType::Chat, TypingMessageAction::SendMessage, destAddress_);
    }
    else if (cmd == KeyCommand::Down) {
        handleScrollDown();
    }
    else if (cmd == KeyCommand::Up) {
        handleScrollUp();
    }
    else {
        UIPageBase::onKeyCommand(cmd);
    }
}

void UIPageChat::handleScrollDown()
{
    std::lock_guard g(mutexMsg_);

    if (lineOffset_ + ctx_->maxCountLines < messages_.totalCountLines) {
        lineOffset_++;
    }
    else if (canLoadNextMsg_ && lastMsgIndex_ > 0) {
        lastMsgIndex_--;
        auto oldTotalLines = messages_.totalCountLines;
        messages_ = loadMessages(lastMsgIndex_, destAddress_, countMessagesPerPage_);

        if (messages_.totalCountLines > oldTotalLines) {
            lineOffset_ += messages_.totalCountLines - oldTotalLines;
        }

        canLoadNextMsg_ = lastMsgIndex_ > 0;
        canLoadPrevMsg_ = messages_.hasMore;
    }
}

void UIPageChat::handleScrollUp()
{
    std::lock_guard g(mutexMsg_);

    if (lineOffset_ > 0) {
        lineOffset_--;
    }
    else if (canLoadPrevMsg_) {
        lastMsgIndex_++;
        messages_ = loadMessages(lastMsgIndex_, destAddress_, countMessagesPerPage_);

        lineOffset_ = 0;

        canLoadNextMsg_ = true;
        canLoadPrevMsg_ = messages_.hasMore;
    }
}

void UIPageChat::onIncomingMessage(const std::string& message, uint16_t senderAddress, uint16_t destAddress)
{
    bool isForMe = destAddress == selfAddress_;
    uint16_t cmpAddress = isForMe ? senderAddress : destAddress;
    if (cmpAddress != destAddress_) {
        return; // Ignore messages not for the current chat
    }

    ContactList contacts = ctx_->contactsManager->getContacts();
    ChatMessage msg;
    msg.address = senderAddress;
    msg.msg = message;
    msg.status = MessageStatus::New;
    msg.timestamp = std::time(nullptr);
    auto fmtMsg = formatMessage(msg, contacts);

    {
        std::lock_guard g(mutexMsg_);
        messages_.totalCountLines += (uint16_t)fmtMsg.size();
        messages_.list.push_back(std::move(fmtMsg));

        if (messages_.totalCountLines > ctx_->maxCountLines) {
            lineOffset_ = messages_.totalCountLines - ctx_->maxCountLines;
        }
    }
}

void UIPageChat::init(uint16_t destAddress)
{
    std::lock_guard g(mutexMsg_);
    destAddress_ = destAddress;
    lastMsgIndex_ = 0;   
    messages_ = loadMessages(lastMsgIndex_, destAddress, countMessagesPerPage_);

    LOG_DBG("init: loaded %zu messages, totalLines=%u, maxCountLines=%u",
        messages_.list.size(), messages_.totalCountLines, ctx_->maxCountLines);

    canLoadPrevMsg_ = messages_.hasMore;
    canLoadNextMsg_ = false;

    if (messages_.totalCountLines > ctx_->maxCountLines) {
        lineOffset_ = messages_.totalCountLines - ctx_->maxCountLines;
    }
    else {
        lineOffset_ = 0;
    }

    LOG_DBG("init: set lineOffset_=%u", lineOffset_);
}

UIPageChat::MessageList UIPageChat::loadMessages(uint16_t startMsgIndex, uint16_t destAddress, uint16_t maxCountMessages)
{
    uint16_t countMessages = maxCountMessages + 1; // Load one extra message to check if there are more messages
    auto messages = ctx_->chatManager->getMessages(destAddress, startMsgIndex, countMessages);
    auto contacts = ctx_->contactsManager->getContacts();

    LOG_DBG("loadMessages: requested %u messages starting from index %u, got %zu messages",
        maxCountMessages, startMsgIndex, messages.size());
    if (!messages.empty()) {
        LOG_DBG("First message: %s", messages.front().msg.c_str());
        LOG_DBG("Last message: %s", messages.back().msg.c_str());
    }

    MessageList res;
    res.hasMore = messages.size() > maxCountMessages;

    size_t startIdx = 0;
    if (messages.size() > maxCountMessages) {
        startIdx = messages.size() - maxCountMessages;
    }

    for (size_t i = startIdx; i < messages.size(); ++i) {
        auto& msg = messages[i];
        auto fmtMsg = formatMessage(msg, contacts);
        res.totalCountLines += (uint16_t)fmtMsg.size();
        res.list.push_back(std::move(fmtMsg));
    }

    return res;
}

UIPageChat::FormatedMessage UIPageChat::formatMessage(const ChatMessage& srcMsg, const ContactList& contacts)
{
    FormatedMessage msg;
    
    std::string contactName;
    if (srcMsg.address == BROADCAST_ADDRESS) {
        contactName = "Широковещательная";
    }
    else if (srcMsg.address == 0) {
        contactName = "Неизвестный";
    }
    else if (srcMsg.address == selfAddress_) {
        contactName = "Я";
    }
    else {
        auto it = std::find_if(contacts.begin(), contacts.end(),
            [&srcMsg](const Contact& c) { return c.address == srcMsg.address; });
        std::string findedContactName = it != contacts.end() ? it->name : "";
        contactName = findedContactName.empty() ? std::to_string(srcMsg.address) : findedContactName;
    }
    std::string text = contactName + ":" + srcMsg.msg;
    msg = utils::splitUtf8String(text, ctx_->maxLineChars);

    return msg;
}
