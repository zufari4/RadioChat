#pragma once

#include "UIPageBase.h"
#include "../Chat/ChatMessage.h"
#include "../Contacts/ContactsManger.h"
#include <mutex>
#include <vector>
#include <tuple>

class UIPageChat : public UIPageBase
{
public:
    UIPageChat(UIPageType parent, const UIContext* context);
    ~UIPageChat();
    
    void draw() override;
    void onKeyCommand(KeyCommand cmd) override;
    void onIncomingMessage(const std::string& message, uint16_t senderAddress, uint16_t destAddress);
    void init(uint16_t destAddress);

private:
    using FormatedMessage = std::vector<std::string>;
    using tdMessages = std::vector<FormatedMessage>;
    struct MessageList {
        tdMessages list;
        uint16_t totalCountLines = 0;
    };
    MessageList loadMessages(uint16_t startMsgIndex, uint16_t destAddress, uint16_t maxCountMessages);
    FormatedMessage formatMessage(const ChatMessage& srcMsg, const ContactList& contacts);
    void handleScrollDown();
    void handleScrollUp();

    uint16_t lastMsgIndex_;
    uint16_t lineOffset_;
    std::recursive_mutex mutexMsg_;
    MessageList messages_;
    bool canLoadPrevMsg_;
    bool canLoadNextMsg_;
    const uint16_t maxCountMessages_;
    uint16_t destAddress_;
    const uint16_t selfAddress_;
};
