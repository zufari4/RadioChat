#include "UIPageChat.h"
#include "Utils.h"
#include "Configuration.h"
#include "Display.h"

UIPageChat::UIPageChat(const UISettings& settings, Display* display)
    : UIPageBase(UIPageType::TypingMessage, settings, display)
    , typingMessage_{""}
{
}

UIPageChat::~UIPageChat()
{
}

void UIPageChat::draw()
{
    uint8_t y = 0;
    for (int i = 0; i < typingMessage_.size(); ++i) {
        drawText(0, y, typingMessage_[0]);
        y += getTextHeight();
    }
}

void UIPageChat::onKeyUp(uint16_t symbol)
{
    std::string& typingMessage = typingMessage_.back();
    if (utils::utf8_len(typingMessage) >= getMaxStrLen()) {
        typingMessage_.push_back("");
        typingMessage = typingMessage_.back();
    }
    utils::add_to_str(symbol, typingMessage);
}

void UIPageChat::onKeyCommand(KeyCommand cmd)
{
    switch (cmd) {
    case KeyCommand::Backspace: {
        std::string& typingMessage = typingMessage_.back();
        utils::pop_back_utf8(typingMessage);
        if (typingMessage.empty() && typingMessage_.size() > 1) typingMessage_.pop_back();
    }
    case KeyCommand::Enter    :
    case KeyCommand::Left     :
    case KeyCommand::Right    :
    case KeyCommand::Up       :
    case KeyCommand::Down     :
    default: return;
    }
}