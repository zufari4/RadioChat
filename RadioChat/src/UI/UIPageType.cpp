#include "UIPageType.h"

const char* uiPageTypeToStr(UIPageType type)
{
    switch (type) {
    case UIPageType::None:            return "None";
    case UIPageType::Main:            return "Main";
    case UIPageType::ChatSelect:      return "ChatSelect";
    case UIPageType::TypingMessage:   return "TypingMessage";
    case UIPageType::SettingsSelect:  return "SettingsSelect";
    case UIPageType::PropertyList:    return "PropertyList";
    case UIPageType::ChooseOption:    return "ChooseOption";
    case UIPageType::Statistics:      return "Statistics";
    case UIPageType::Monitoring:      return "Monitoring";
    case UIPageType::IncomingMessage: return "IncomingMessage";
    case UIPageType::Contacts:        return "Contacts";
    case UIPageType::Logs:            return "Logs";
    default:                          return "Unknown";
    }
}
