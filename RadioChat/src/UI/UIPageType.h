#pragma once

enum class UIPageType
{
    None,
    Main,
    ChatSelect,
    TypingMessage,
    Statistics,
    Monitoring,
    IncomingMessage,
    Contacts,
    Logs,
    SettingsSelect,
    PropertyList,
    EditProperty,
};

const char* uiPageTypeToStr(UIPageType type);