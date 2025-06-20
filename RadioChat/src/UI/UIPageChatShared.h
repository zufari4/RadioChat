#pragma once

#include "UIPageBase.h"
#include "../Chat/ChatMessage.h"
#include <mutex>
#include <vector>
#include <tuple>

class UIPageChatShared : public UIPageBase
{
public:
    UIPageChatShared(UIPageType parent, const UIContext* context);
    ~UIPageChatShared();
    
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;
    void onIncomingMessage(const std::string& message, uint16_t address);

private:
    using FormatedMessage = std::vector<std::string>;
    using tdMessages = std::vector<FormatedMessage>;
    struct MessageList {
        tdMessages list;
        uint16_t totalCountLines = 0;
    };
    MessageList loadMessages(uint16_t startMsgIndex);
    FormatedMessage formatMessage(const ChatMessage& srcMsg);
    void handleScrollDown();
    void handleScrollUp();

    uint16_t lastMsgIndex_;
    uint16_t lineOffset_;
    std::recursive_mutex mutexMsg_;
    MessageList messages_;
    bool canLoadPrevMsg_;
    bool canLoadNextMsg_;
    const uint16_t maxCountMessages_;
};
