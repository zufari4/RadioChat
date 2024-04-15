#pragma once

#include "UIPageBase.h"
#include <vector>

class UIPageChat: public UIPageBase
{
public:
    UIPageChat(const UISettings& settings, Display* display);
    ~UIPageChat();
    void draw() override;
    void onChar(uint16_t symbol) override;
    void onKeyCommand(KeyCommand cmd) override;

private:
    std::vector<std::string> typingMessage_;
};