#include "UIPageIncomingMessage.h"
#include "../Display/Display.h"

UIPageIncomingMessage::UIPageIncomingMessage(const UIContext *context, const std::string& message, uint16_t address)
    : UIPageBase(UIPageType::IncomingMessage, context)
    , message_(splitMessage(message))
    , address_(address)
{
}

UIPageIncomingMessage::~UIPageIncomingMessage()
{
}

void UIPageIncomingMessage::draw()
{
    ctx_->display->drawStr(0, 0, "Сообщение от " + std::to_string(address_));
    uint8_t y = ctx_->textHeight;

    for (const auto& line : message_) {
        ctx_->display->drawStr(0, y, line);
        y += ctx_->textHeight;
    }
}

std::vector<std::string> UIPageIncomingMessage::splitMessage(const std::string &message) const
{
    std::vector<std::string> lines;
    std::string currentLine;
    size_t lineLength = 0;

    for (const char &c : message) {
        currentLine += c;
        lineLength++;
        if (lineLength >= ctx_->maxStrLen)
        {
            lines.push_back(currentLine);
            currentLine.clear();
            lineLength = 0;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

void UIPageIncomingMessage::onKeyCommand(KeyCommand cmd)
{
    switch (cmd) {
    case KeyCommand::Escape:
        if (cmd == KeyCommand::Escape) {
            setExitStatus(ExitStatus::Cancel);
            return;
        }
    default:
        break;
    }
}
