#include "UIPageChatContact.h"
#include "../Chat/ChatManager.h"
#include "../Display/Display.h"
#include "../Radio/Lora.h"

UIPageChatContact::UIPageChatContact(UIPageType parent, const UIContext* context)
    : UIPageBase(UIPageType::ChatContact, parent, context)
    , address_(0)
    , currentPage_(0)
    , lineOffset_(0)
{
    std::lock_guard g(mutexMsg_);
    messages_ = loadMessages(currentPage_);
}

UIPageChatContact::~UIPageChatContact() = default;

void UIPageChatContact::draw()
{
    std::lock_guard g(mutexMsg_);

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
            if (offsetY + ctx_->textHeight < dispH) {
                offsetY += ctx_->textHeight;
                lineIndex++;
            }
            else {
                // If we reach the end of the display, stop drawing
                return;
            }
        }
    }
}

void UIPageChatContact::onKeyCommand(KeyCommand cmd)
{
    if (cmd == KeyCommand::Enter) {
        ctx_->showPageTypingMessage(BROADCAST_ADDRESS);
    }
    else if (cmd == KeyCommand::Down) {
        std::lock_guard g(mutexMsg_);

        if (lineOffset_ < messages_.totalCountLines) {
            lineOffset_++;
        }
    }
    else if (cmd == KeyCommand::Up) {
        std::lock_guard g(mutexMsg_);
        if (lineOffset_ > 0) {
            lineOffset_--;
        }
        else {
            currentPage_++;
            lineOffset_ = 0;
            messages_ = loadMessages(currentPage_);
        }
    }
    else {
        UIPageBase::onKeyCommand(cmd);
    }
}

void UIPageChatContact::setAddress(uint16_t address)
{
    address_ = address;
}

UIPageChatContact::MessageList UIPageChatContact::loadMessages(uint16_t page)
{
    uint16_t maxCountMessages = ctx_->maxCountLines / 2; // address + message
    auto messages = ctx_->chatManager->getMessages(BROADCAST_ADDRESS, page * maxCountMessages, maxCountMessages);
    MessageList res;

    for (const auto& msg : messages) {
        auto fmtMsg = formatMessage(msg);
        res.totalCountLines += (uint16_t)fmtMsg.size();
        res.list.push_back(std::move(fmtMsg));
    }

    return res;
}

UIPageChatContact::FormatedMessage UIPageChatContact::formatMessage(const ChatMessage& srcMsg)
{
    FormatedMessage msg;
    msg.push_back(std::to_string(srcMsg.address));
    std::string text = srcMsg.msg;

    while (!text.empty()) {
        if (text.size() > ctx_->maxLineChars) {
            msg.push_back(text.substr(0, ctx_->maxLineChars));
            text = text.substr(0, ctx_->maxLineChars);
        } else {
            msg.push_back(text);
            text.clear();
        }
    }
    return msg;
}
