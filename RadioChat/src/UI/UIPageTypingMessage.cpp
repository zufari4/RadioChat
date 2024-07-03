#include "UIPageTypingMessage.h"
#include "../Utils.h"
#include "../Configuration.h"
#include "../Display/Display.h"
#include "../Logger/Logger.h"

UIPageTypingMessage::UIPageTypingMessage(const UIContext* context)
    : UIPageBase(UIPageType::Chat, context)
    , typingMessage_{""}
    , carriageChar_(context->uiSettings.carriageChar)
    , carriageVisible_(false)
    , carriageShowTime_(context->uiSettings.carriageShowTime)
{
}

UIPageTypingMessage::~UIPageTypingMessage()
{
}

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
    }
    case KeyCommand::Enter    :
    case KeyCommand::Left     :
    case KeyCommand::Right    :
    case KeyCommand::Up       :
    case KeyCommand::Down     :
    default: return;
    }
}