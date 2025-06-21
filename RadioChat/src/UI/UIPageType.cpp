#include "UIPageType.h"

const char* uiPageTypeToStr(UIPageType type)
{
    switch (type) {
    case UIPageType::None:            return "None";
    case UIPageType::Main:            return "Main";
    case UIPageType::ChatSelect:      return "ChatSelect";
    case UIPageType::Chat:            return "Chat";
    case UIPageType::TypingMessage:   return "TypingMessage";
    case UIPageType::SettingsSelect:  return "SettingsSelect";
    case UIPageType::PropertyList:    return "PropertyList";
    case UIPageType::EditProperty:    return "EditProperty";
    case UIPageType::Statistics:      return "Statistics";
    case UIPageType::Monitoring:      return "Monitoring";
    case UIPageType::IncomingMessage: return "IncomingMessage";
    case UIPageType::Contacts:        return "Contacts";
    case UIPageType::Logs:            return "Logs";
    case UIPageType::ContactActions:  return "ContactActions";
    case UIPageType::Ping:            return "Ping";
    default:                          return "Unknown";
    }
}
