#include "MessageStatus.h"

std::string msgStatusToString(MessageStatus status)
{
    switch (status)
    {
    case MessageStatus::Sending:   return "Sending";
    case MessageStatus::Sended:    return "Sended";
    case MessageStatus::Delivered: return "Delivered";
    case MessageStatus::New:       return "New";
    case MessageStatus::Readed:    return "Readed";
    case MessageStatus::Error:     return "Error";
    default:        return "Unknown";
    }
}
