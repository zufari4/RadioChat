#pragma once

#include "UISettings.h"
#include "KeyCommand.h"
#include <memory>

class Display;
class UIPageBase;

class UI
{
public:
    UI();
    ~UI();
    void init(const UISettings& settings, Display* display);
    void draw();
    void onChar(uint16_t symbol);
    void onKeyCommand(KeyCommand cmd);

private:
    Display* display_;
    std::unique_ptr<UIPageBase> currentPage_;
};