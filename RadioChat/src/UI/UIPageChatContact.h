#pragma once

#include "UIPageBase.h"
#include "../Chat/ChatMessage.h"
#include <mutex>
#include <vector>
#include <tuple>

class UIPageChatContact : public UIPageBase
{
public:
    UIPageChatContact(UIPageType parent, const UIContext* context);
    ~UIPageChatContact();
    
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;
    void setAddress(uint16_t address);

private:
    using FormatedMessage = std::vector<std::string>;
    using tdMessages = std::vector<FormatedMessage>;
    struct MessageList {
        tdMessages list;
        uint16_t totalCountLines = 0;
    };
    uint16_t address_ = 0;
    MessageList loadMessages(uint16_t page);
    FormatedMessage formatMessage(const ChatMessage& srcMsg);
    uint16_t currentPage_;
    uint16_t lineOffset_;
    std::recursive_mutex mutexMsg_;
    MessageList messages_;
};
