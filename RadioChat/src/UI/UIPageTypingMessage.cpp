#include "UIPageTypingMessage.h"
#include "../Utils.h"
#include "../Configuration.h"
#include "../Display/Display.h"
#include "../Logger/Logger.h"
#include "../Radio/Radio.h"

UIPageTypingMessage::UIPageTypingMessage(UIPageType parent, const UIContext* context)
    : UIPageBase(UIPageType::TypingMessage, parent, context)
    , address_(0)
    , typingMessage_{""}
    , carriageChar_(context->uiSettings.carriageChar)
    , carriageVisible_(false)
    , carriageShowTime_(context->uiSettings.carriageShowTime)
{
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

    std::string& typingMessage = typingMessage_.back();
    int len = utils::utf8_len(typingMessage);

    if (len < ctx_->maxStrLen) {
        utils::add_to_str(symbol, typingMessage);
    }

    if (len+1 >= ctx_->maxStrLen) {
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
        utils::pop_back_utf8(typingMessage);
        if (typingMessage.empty() && typingMessage_.size() > 1) {
            //LOG_INF("Remove line");
            typingMessage_.pop_back();
        }
        break;
    }
    case KeyCommand::Enter: {
        std::string message = getMessage();
        if (message.empty()) {
            return;
        }
        ctx_->radio->sendText(message, address_);
        break;
    }
    default: 
        UIPageBase::onKeyCommand(cmd);
    }
}

std::string UIPageTypingMessage::getMessage()
{
    std::string message;
    std::lock_guard guard(msgMutex_);

    if (typingMessage_.empty()) {
        return message;
    }

    for (const auto& line : typingMessage_) {
        message += line;
        if (!std::isspace(line.back())) {
            message += ' ';
        }
    }

    if (std::isspace(message.back())) {
        message.pop_back();
    }

    return message;
}

void UIPageTypingMessage::setAddress(uint16_t address)
{
    address_ = address;
}
