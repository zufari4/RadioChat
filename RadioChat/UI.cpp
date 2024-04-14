#include "UI.h"
#include "Display.h"
#include "UIPageChat.h"
#include "UIPageTypingMessage.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::init(const UISettings& settings, Display* display)
{
    display_ = display;
    currentPage_ = std::make_unique<UIPageTypingMessage>(settings, display_);
}

void UI::draw()
{
    display_->clear();
    currentPage_->draw();
    display_->flush();
}

void UI::onKeyUp(uint16_t symbol)
{
    currentPage_->onKeyUp(symbol);
}

void UI::onKeyCommand(KeyCommand cmd)
{
    currentPage_->onKeyCommand(cmd);
}