#pragma once

#include <vector>
#include "ChatMessage.h"

class Settings;

class ChatManager
{
public:
    ChatManager();
    ~ChatManager();
    void init(Settings& settings);
    std::vector<ChatMessage> getMessages(uint16_t address, uint32_t start, uint32_t count) const;
    void storeMessage(uint16_t senderAddress, uint16_t destAddress, const std::string& msg, MessageStatus status);
    void clearSharedChat();
private:
    std::string getChatFileName(uint16_t address) const;
    ChatMessage parseMessage(const std::string& line) const;

    std::string path_;
    std::string sharedFileName_;
    uint16_t selfAddress_;
};