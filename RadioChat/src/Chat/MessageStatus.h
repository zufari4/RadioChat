#pragma once

#include <cstdint>
#include <string>

enum class MessageStatus : uint8_t
{
    Sending = 0,
    Sended = 1,
    Delivered = 2,
    New = 3,
    Readed = 4,
    Error = 5
};

std::string msgStatusToString(MessageStatus status);
