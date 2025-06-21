#include "UIPagePing.h"
#include "../Contacts/ContactsManger.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"

UIPagePing::UIPagePing(UIPageType parent, const UIContext* context)
    : BaseMenu(UIPageType::Ping, parent, context)
    , address_(0)
{
}

UIPagePing::~UIPagePing() = default;

void UIPagePing::setAddress(uint16_t address)
{
    address_ = address;
}
