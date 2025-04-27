#pragma once

#include "UIPageBase.h"
#include <vector>

class UIPageIncomingMessage: public UIPageBase
{
public:
    UIPageIncomingMessage(UIPageType parent, const UIContext* context);
    ~UIPageIncomingMessage();
    void draw() override;
    void setMessage(const std::string& message, uint16_t address);

private:
    std::vector<std::string> splitMessage(const std::string& message) const;
    std::vector<std::string> message_;
    uint16_t address_ = 0u;
};