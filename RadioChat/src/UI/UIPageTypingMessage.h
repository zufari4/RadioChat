#pragma once

#include "UIPageBase.h"
#include "TypingMessageAction.h"
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
    void setAction(TypingMessageAction action);

private:
    std::string getFullMessage();
    void resetMessage();
    void sendMessage();
    void storeContact();
    void editContact();

    using Clock = std::chrono::steady_clock;
    uint16_t address_;
    TypingMessageAction action_;
    std::vector<std::string> typingMessage_;
    char carriageChar_;
    bool carriageVisible_;
    Clock::time_point nextCarriageShow_;
    std::chrono::milliseconds carriageShowTime_;
    std::mutex msgMutex_;
    size_t currentMessageSize_;
};