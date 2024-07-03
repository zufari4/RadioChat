#include "UI.h"
#include "UIPageChat.h"
#include "UIPageTypingMessage.h"
#include "UIPageMain.h"
#include "../Logger/Logger.h"
#include "../Display/Display.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::init(const UIContext& context)
{
    LOG_INF("-- Initialize UI --");

    ctx_ = context;
    ctx_.maxStrLen = ctx_.display->getDisplayWidth()/ctx_.display->getMaxCharWidth();
    ctx_.textHeight = ctx_.uiSettings.textHeight;
    ctx_.maxCountLines = ctx_.display->getDisplayHeight()/ctx_.textHeight;

    LOG_DBG("maxStrLen %u textHeight %u maxCountLines %u", ctx_.maxStrLen, ctx_.textHeight, ctx_.maxCountLines);

    currentPage_ = std::make_unique<UIPageMain>(&ctx_);
}

void UI::draw()
{
    ctx_.display->clear();
    currentPage_->draw();
    ctx_.display->flush();
}

void UI::onChar(uint16_t symbol)
{
    currentPage_->onChar(symbol);
}

void UI::onKeyCommand(KeyCommand cmd)
{
    currentPage_->onKeyCommand(cmd);
}