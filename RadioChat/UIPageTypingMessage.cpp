#include "UIPageTypingMessage.h"
#include "Utils.h"
#include "Configuration.h"
#include "Display.h"
#include "Logger.h"

UIPageTypingMessage::UIPageTypingMessage(const UISettings& settings, Display* display)
    : UIPageBase(UIPageType::Chat, settings, display)
    , typingMessage_{""}
{
}

UIPageTypingMessage::~UIPageTypingMessage()
{
}

void UIPageTypingMessage::draw()
{
    uint8_t y = 0;
    for (const auto& line : typingMessage_) {
        drawText(0, y, line);
        y += getTextHeight();
    }
}

void UIPageTypingMessage::onKeyUp(uint16_t symbol)
{
    std::string* typingMessage = &typingMessage_.back();
    if (utils::utf8_len(*typingMessage) >= getMaxStrLen()) {
        if (typingMessage_.size() >= getMaxCountLines()) {
            LOG("Text is full\n");
            return;
        }
        LOG("New line\n");
        typingMessage_.emplace_back();
        typingMessage = &typingMessage_.back();
    }
    utils::add_to_str(symbol, *typingMessage);
}

void UIPageTypingMessage::onKeyCommand(KeyCommand cmd)
{
    switch (cmd) {
    case KeyCommand::Backspace: {
        std::string& typingMessage = typingMessage_.back();
        utils::pop_back_utf8(typingMessage);
        if (typingMessage.empty() && typingMessage_.size() > 1) {
            LOG("Remove line\n");
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