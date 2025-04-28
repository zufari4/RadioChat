#pragma once

#include "UIPageBase.h"
#include <vector>
#include <chrono>
#include <mutex>
#include <stdint.h>

class UIPageTypingMessage: public UIPageBase
{
public:
    UIPageTypingMessage(UIPageType parent, const UIContext* context);
    ~UIPageTypingMessage();
    void draw() override;
    void onChar(uint16_t symbol) override;
    void onKeyCommand(KeyCommand cmd) override;
    void setAddress(uint16_t address);

private:
    std::string getMessage();
    void resetMessage();

    using Clock = std::chrono::steady_clock;
    uint16_t address_;
    std::vector<std::string> typingMessage_;
    char carriageChar_;
    bool carriageVisible_;
    Clock::time_point nextCarriageShow_;
    std::chrono::milliseconds carriageShowTime_;
    std::mutex msgMutex_;
};