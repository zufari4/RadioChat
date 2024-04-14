#pragma once

#include "UIPageBase.h"
#include <vector>

class UIPageTypingMessage: public UIPageBase
{
public:
    UIPageTypingMessage(const UISettings& settings, Display* display);
    ~UIPageTypingMessage();
    void draw() override;
    void onKeyUp(uint16_t symbol) override;
    void onKeyCommand(KeyCommand cmd) override;

private:
    std::vector<std::string> typingMessage_;
};;