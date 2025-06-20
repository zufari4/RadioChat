#include "UIPageChatShared.h"
#include "../Chat/ChatManager.h"
#include "../Display/Display.h"
#include "../Radio/Lora.h"
#include "../Utils.h"

UIPageChatShared::UIPageChatShared(UIPageType parent, const UIContext* context)
    : UIPageBase(UIPageType::ChatShared, parent, context)
    , lastMsgIndex_(0)
    , lineOffset_(0)
    , canLoadNextMsg_(true)
    , canLoadPrevMsg_(true)
    , maxCountMessages_(context->maxCountLines)
{
    std::lock_guard g(mutexMsg_);
    messages_ = loadMessages(lastMsgIndex_);
    canLoadNextMsg_ = messages_.list.size() >= maxCountMessages_;
    canLoadPrevMsg_ = lastMsgIndex_ > 0;
}

UIPageChatShared::~UIPageChatShared() = default;

void UIPageChatShared::draw()
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

void UIPageChatShared::onKeyCommand(KeyCommand cmd)
{
    if (cmd == KeyCommand::Enter) {
        ctx_->showPageTypingMessage(BROADCAST_ADDRESS);
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

void UIPageChatShared::handleScrollDown()
{
    std::lock_guard g(mutexMsg_);

    if (lineOffset_ + ctx_->maxCountLines < messages_.totalCountLines) {
        lineOffset_++;
    }
    else if (canLoadNextMsg_ && lastMsgIndex_ > 0) {
        lastMsgIndex_--;
        auto oldTotalLines = messages_.totalCountLines;
        messages_ = loadMessages(lastMsgIndex_);

        if (messages_.totalCountLines > oldTotalLines) {
            lineOffset_ += messages_.totalCountLines - oldTotalLines;
        }

        canLoadNextMsg_ = lastMsgIndex_ > 0;
        canLoadPrevMsg_ = messages_.list.size() >= maxCountMessages_;
    }
}

void UIPageChatShared::handleScrollUp()
{
    std::lock_guard g(mutexMsg_);

    if (lineOffset_ > 0) {
        lineOffset_--;
    }
    else if (canLoadPrevMsg_) {
        lastMsgIndex_++;
        messages_ = loadMessages(lastMsgIndex_);

        lineOffset_ = 0;

        canLoadNextMsg_ = true;
        canLoadPrevMsg_ = messages_.list.size() >= maxCountMessages_;
    }
}

void UIPageChatShared::onIncomingMessage(const std::string& message, uint16_t address)
{
    ChatMessage msg;
    msg.address = address;
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

UIPageChatShared::MessageList UIPageChatShared::loadMessages(uint16_t startMsgIndex)
{
    auto messages = ctx_->chatManager->getMessages(BROADCAST_ADDRESS, startMsgIndex, maxCountMessages_);
    MessageList res;

    for (const auto& msg : messages) {
        auto fmtMsg = formatMessage(msg);
        res.totalCountLines += (uint16_t)fmtMsg.size();
        res.list.push_back(std::move(fmtMsg));
    }

    return res;
}

UIPageChatShared::FormatedMessage UIPageChatShared::formatMessage(const ChatMessage& srcMsg)
{
    FormatedMessage msg;
    std::string text = std::to_string(srcMsg.address) + ": " + srcMsg.msg;
    msg = utils::splitUtf8String(text, ctx_->maxLineChars);

    return msg;
}
