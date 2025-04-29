#include "UIPageIncomingMessage.h"
#include "../Display/Display.h"
#include "../Utils.h"

UIPageIncomingMessage::UIPageIncomingMessage(UIPageType parent, const UIContext *context)
    : UIPageBase(UIPageType::IncomingMessage, parent, context)
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

void UIPageIncomingMessage::setMessage(const std::string& message, uint16_t address)
{
    message_ = utils::splitUtf8String(message, ctx_->maxLineChars);
    address_ = address;
}
