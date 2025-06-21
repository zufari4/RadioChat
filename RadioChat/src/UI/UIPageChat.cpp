#include "UIPageChat.h"
#include "../Chat/ChatManager.h"
#include "../Contacts/ContactsManger.h"
#include "../Display/Display.h"
#include "../Radio/Radio.h"
#include "../Utils.h"

UIPageChat::UIPageChat(UIPageType parent, const UIContext* context)
    : UIPageBase(UIPageType::Chat, parent, context)
    , lastMsgIndex_(0)
    , lineOffset_(0)
    , canLoadPrevMsg_(true)
    , canLoadNextMsg_(true)
    , maxCountMessages_(context->maxCountLines)
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
        messages_ = loadMessages(lastMsgIndex_, destAddress_, maxCountMessages_);

        if (messages_.totalCountLines > oldTotalLines) {
            lineOffset_ += messages_.totalCountLines - oldTotalLines;
        }

        canLoadNextMsg_ = lastMsgIndex_ > 0;
        canLoadPrevMsg_ = messages_.list.size() >= maxCountMessages_;
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
        messages_ = loadMessages(lastMsgIndex_, destAddress_, maxCountMessages_);

        lineOffset_ = 0;

        canLoadNextMsg_ = true;
        canLoadPrevMsg_ = messages_.list.size() >= maxCountMessages_;
    }
}

void UIPageChat::onIncomingMessage(const std::string& message, uint16_t senderAddress, uint16_t destAddress)
{
    bool isForMe = destAddress == selfAddress_;
    uint16_t cmpAddress = isForMe ? senderAddress : destAddress;
    if (cmpAddress != destAddress_) {
        return; // Ignore messages not for the current chat
    }

    ChatMessage msg;
    msg.address = senderAddress;
    msg.msg = message;
    msg.status = MessageStatus::New;
    msg.timestamp = std::time(nullptr);
    auto fmtMsg = formatMessage(msg);

    {
        std::lock_guard g(mutexMsg_);
        messages_.totalCountLines += (uint16_t)fmtMsg.size();
        messages_.list.push_back(std::move(fmtMsg));

        if (messages_.totalCountLines > ctx_->maxCountLines) {
            lineOffset_ = messages_.totalCountLines - ctx_->maxCountLines;
        }

        canLoadPrevMsg_ = messages_.list.size() >= maxCountMessages_;
    }
}

void UIPageChat::init(uint16_t destAddress)
{
    std::lock_guard g(mutexMsg_);
    destAddress_ = destAddress;
    lastMsgIndex_ = 0;   
    messages_ = loadMessages(lastMsgIndex_, destAddress, maxCountMessages_);
    canLoadNextMsg_ = messages_.list.size() >= maxCountMessages_;
    canLoadPrevMsg_ = lastMsgIndex_ > 0;
}

UIPageChat::MessageList UIPageChat::loadMessages(uint16_t startMsgIndex, uint16_t destAddress, uint16_t maxCountMessages)
{
    auto messages = ctx_->chatManager->getMessages(destAddress, startMsgIndex, maxCountMessages);
    MessageList res;

    for (const auto& msg : messages) {
        auto fmtMsg = formatMessage(msg);
        res.totalCountLines += (uint16_t)fmtMsg.size();
        res.list.push_back(std::move(fmtMsg));
    }

    return res;
}

UIPageChat::FormatedMessage UIPageChat::formatMessage(const ChatMessage& srcMsg)
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
        auto contact = ctx_->contactsManager->getContact(srcMsg.address);
        contactName = contact.name.empty() ? std::to_string(srcMsg.address) : contact.name;
    }
    std::string text = contactName + ":" + srcMsg.msg;
    msg = utils::splitUtf8String(text, ctx_->maxLineChars);

    return msg;
}
