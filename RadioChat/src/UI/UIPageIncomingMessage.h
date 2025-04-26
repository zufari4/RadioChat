#pragma once

#include "UIPageBase.h"
#include <vector>

class UIPageIncomingMessage: public UIPageBase
{
public:
    UIPageIncomingMessage(const UIContext* context, const std::string& message, uint16_t address);
    ~UIPageIncomingMessage();
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;

private:
    std::vector<std::string> splitMessage(const std::string& message) const;
    std::vector<std::string> message_;
    uint16_t address_;
};