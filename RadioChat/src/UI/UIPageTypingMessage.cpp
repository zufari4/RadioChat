#include "UIPageTypingMessage.h"
#include "../Utils.h"
#include "../Configuration.h"
#include "../Display/Display.h"
#include "../Logger/Logger.h"
#include "../Radio/Radio.h"

UIPageTypingMessage::UIPageTypingMessage(UIPageType parent, const UIContext* context)
    : UIPageBase(UIPageType::TypingMessage, parent, context)
    , address_(0)
    , carriageChar_(context->uiSettings.carriageChar)
    , carriageVisible_(false)
    , carriageShowTime_(context->uiSettings.carriageShowTime)
    , currentMessageSize_(0)
{
    resetMessage();
}

UIPageTypingMessage::~UIPageTypingMessage() = default;

void UIPageTypingMessage::draw()
{
    std::lock_guard guard(msgMutex_);
    
    uint8_t y = 0;

    if (Clock::now() > nextCarriageShow_) {
        nextCarriageShow_ = Clock::now() + carriageShowTime_;
        carriageVisible_ = !carriageVisible_;
    }

    for (int i = 0; i < typingMessage_.size(); ++i) {
        if (carriageVisible_ && (i + 1 == typingMessage_.size())) {
            ctx_->display->drawStr(0, y, typingMessage_[i] + carriageChar_);
        }
        else {
            ctx_->display->drawStr(0, y, typingMessage_[i]);
            y += ctx_->textHeight;
        }
    }
}

void UIPageTypingMessage::onChar(uint16_t symbol)
{
    std::lock_guard guard(msgMutex_);

    uint8_t maxMessageSize = ctx_->radio->getMaxMessageSize();
    std::string utf8symbol = utils::to_str(symbol);

    if (currentMessageSize_ + utf8symbol.size() > maxMessageSize) {
        LOG_DBG("Message is too long. Max size %u", maxMessageSize);
        return;
    }

    std::string& typingMessage = typingMessage_.back();
    int len = utils::utf8_len(typingMessage);

    if (len < ctx_->maxLineChars) {
        typingMessage += utf8symbol;
        currentMessageSize_ += utf8symbol.size();
    }

    if (len+1 >= ctx_->maxLineChars) {
        if (typingMessage_.size() < ctx_->maxCountLines) {
            //LOG_INF("New line");
            typingMessage_.emplace_back();
        }
        else {
            //LOG_INF("Text is full");  
        }
    }

    nextCarriageShow_ = Clock::now() + carriageShowTime_;
    carriageVisible_ = true;
}

void UIPageTypingMessage::onKeyCommand(KeyCommand cmd)
{
    switch (cmd) {
    case KeyCommand::Backspace: {
        std::lock_guard guard(msgMutex_);

        std::string& typingMessage = typingMessage_.back();
        uint8_t removedBytes = utils::pop_back_utf8(typingMessage);
        currentMessageSize_ -= removedBytes;

        if (typingMessage.empty() && typingMessage_.size() > 1) {
            //LOG_INF("Remove line");
            typingMessage_.pop_back();

        }
        break;
    }
    case KeyCommand::Enter: {
        std::string message = getFullMessage();
        if (message.empty()) {
            return;
        }
        ctx_->radio->sendText(message, address_);
        resetMessage();
        break;
    }
    default: 
        UIPageBase::onKeyCommand(cmd);
    }
}

std::string UIPageTypingMessage::getFullMessage()
{
    std::string message;
    std::lock_guard guard(msgMutex_);

    if (typingMessage_.empty()) {
        return message;
    }

    for (const auto& line : typingMessage_) {
        message += line;
    }

    return message;
}

void UIPageTypingMessage::setAddress(uint16_t address)
{
    address_ = address;
}

void UIPageTypingMessage::resetMessage()
{
    std::lock_guard guard(msgMutex_);
    typingMessage_ = { "" };
}
