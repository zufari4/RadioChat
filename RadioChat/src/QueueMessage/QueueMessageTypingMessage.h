#pragma once 

#include "QueueMessageBase.h"
#include "../UI/UIPageType.h"
#include "../UI/TypingMessageAction.h"

#include <stdint.h>

class QueueMessageTypingMessage : public QueueMessageBase
{
public:
    QueueMessageTypingMessage(UIPageType parent, TypingMessageAction action, uint16_t destinationAddress);
    ~QueueMessageTypingMessage();
    uint16_t getAddress() const;
    UIPageType getParentPage() const;
    TypingMessageAction getAction() const;

private:
    UIPageType parent_;
    TypingMessageAction action_;
    uint16_t destinationAddress_;
};