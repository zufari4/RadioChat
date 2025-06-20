#include "ChatManager.h"
#include "../Settings/Settings.h"
#include "../Flash/Flash.h"
#include "../Configuration.h"
#include "../Utils.h"
#include "../Logger/Logger.h"
#include "../Radio/Lora.h"
#include <sstream>

ChatManager::ChatManager() = default;
ChatManager::~ChatManager() = default;

void ChatManager::init(Settings& settings)
{
    LOG_INF("-- Initializing ChatManager --");
    auto props = settings.chat();
    path_ = settings.get_s(PropertyType::eChatPath, props);
    sharedFileName_ = settings.get_s(PropertyType::eChatSharedFilename, props);
    FLASH.mkdir(STORAGE_DIR + std::string("/") + path_);
}

std::vector<ChatMessage> ChatManager::getMessages(uint16_t address, uint32_t start, uint32_t count) const
{
    std::string filename = getChatFileName(address);
    auto lines = FLASH.readLastNLines(filename, start, count);
    std::vector<ChatMessage> res;

    for (const auto& line : lines) {
        ChatMessage message = parseMessage(line);
        if (!message.msg.empty()) {
            res.push_back(std::move(message));
        }
    }

    return res;
}

void ChatManager::storeMessage(uint16_t senderAddress, uint16_t destAddress, const std::string& msg, MessageStatus status)
{
    std::string filename = getChatFileName(destAddress);
    std::vector<char> buffer(512);
    snprintf(buffer.data(), buffer.size(), "%u\t%s\t%u\t%llu\n", senderAddress, msg.c_str(), static_cast<uint8_t>(status), std::time(nullptr));
    if (!FLASH.append(filename, buffer.data())) {
        LOG_ERR("Failed to store message to %s", filename.c_str());
    }
    else {
        LOG_DBG("Message stored: %s", buffer.data());
    }
}

void ChatManager::clearSharedChat()
{
    std::string filename = getChatFileName(BROADCAST_ADDRESS);
    FLASH.remove(filename);
}

std::string ChatManager::getChatFileName(uint16_t address) const
{
    if (address == BROADCAST_ADDRESS) {
        return STORAGE_DIR + std::string("/") + path_ + "/" + sharedFileName_;
    } else {
        return STORAGE_DIR + std::string("/") + path_ + "/" + std::to_string(address) + ".txt";
    }
}

ChatMessage ChatManager::parseMessage(const std::string& line) const
{
    ChatMessage message;
    auto parts = utils::split(line, '\t');

    if (parts.size() < 4) {
        LOG_ERR("Invalid chat message format: %s", line.c_str());
        return message; // Return an empty message
    }

    message.address = static_cast<uint16_t>(std::stoul(parts[0]));
    message.msg = parts[1];
    message.status = static_cast<MessageStatus>(std::stoi(parts[2]));
    message.timestamp = static_cast<std::time_t>(std::stoll(parts[3]));

    return message;
}
