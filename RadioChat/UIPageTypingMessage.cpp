#include "UIPageTypingMessage.h"
#include "Utils.h"
#include "Configuration.h"
#include "Display.h"
#include "Logger.h"

UIPageTypingMessage::UIPageTypingMessage(const UISettings& settings, Display* display)
    : UIPageBase(UIPageType::Chat, settings, display)
    , typingMessage_{""}
    , carriageChar_(settings.carriageChar)
    , carriageVisible_(false)
    , carriageShowTime_(settings.carriageShowTime)
{
}

UIPageTypingMessage::~UIPageTypingMessage()
{
}

void UIPageTypingMessage::draw()
{
    uint8_t y = 0;
    uint8_t h = getTextHeight();

    if (Clock::now() > nextCarriageShow_) {
        nextCarriageShow_ = Clock::now() + carriageShowTime_;
        carriageVisible_ = !carriageVisible_;
    }

    for (int i = 0; i < typingMessage_.size(); ++i) {
        if (carriageVisible_ && (i + 1 == typingMessage_.size())) {
            drawText(0, y, typingMessage_[i] + carriageChar_);
        }
        else {
            drawText(0, y, typingMessage_[i]);
            y += h;
        }
    }
}

void UIPageTypingMessage::onChar(uint16_t symbol)
{
    std::string& typingMessage = typingMessage_.back();
    int len = utils::utf8_len(typingMessage);
    const uint8_t maxLen = getMaxStrLen();

    if (len < maxLen) {
        utils::add_to_str(symbol, typingMessage);
    }

    if (len+1 >= maxLen) {
        if (typingMessage_.size() < getMaxCountLines()) {
            LOG_INF("New line\n");
            typingMessage_.emplace_back();
        }
        else {
            LOG_INF("Text is full\n");  
        }
    }

    nextCarriageShow_ = Clock::now() + carriageShowTime_;
    carriageVisible_ = true;
}

void UIPageTypingMessage::onKeyCommand(KeyCommand cmd)
{
    switch (cmd) {
    case KeyCommand::Backspace: {
        std::string& typingMessage = typingMessage_.back();
        utils::pop_back_utf8(typingMessage);
        if (typingMessage.empty() && typingMessage_.size() > 1) {
            LOG_INF("Remove line\n");
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