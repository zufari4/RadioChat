#include "UI.h"
#include "UIPageChat.h"
#include "UIPageTypingMessage.h"
#include "../Logger/Logger.h"
#include "../Display/Display.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::init(const UISettings& settings, Display* display)
{
    LOG_INF("-- Initialize UI --");
    display_ = display;
    currentPage_ = std::make_unique<UIPageTypingMessage>(settings, display_);
}

void UI::draw()
{
    display_->clear();
    currentPage_->draw();
    display_->flush();
}

void UI::onChar(uint16_t symbol)
{
    currentPage_->onChar(symbol);
}

void UI::onKeyCommand(KeyCommand cmd)
{
    currentPage_->onKeyCommand(cmd);
}