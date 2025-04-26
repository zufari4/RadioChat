#pragma once

#include "UIContext.h"
#include "../Keyboard/KeyCommand.h"
#include <memory>


class UIPageBase;

class UI
{
public:
    UI();
    ~UI();
    void init(const UIContext& context);
    void draw();
    void onChar(uint16_t symbol);
    void onKeyCommand(KeyCommand cmd);
    void onIncomingMessage(const std::string& message, uint16_t address);

private:
    std::mutex pageMutex_;
    UIContext ctx_;
    std::unique_ptr<UIPageBase> currentPage_;
};