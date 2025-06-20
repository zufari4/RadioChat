#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include "MessageStatus.h"

struct ChatMessage
{
    uint16_t address = 0;
    std::string msg;
    MessageStatus status;
    std::time_t timestamp = 0;
};