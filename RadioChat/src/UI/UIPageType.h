#pragma once

enum class UIPageType
{
    None,
    Main,
    ChatSelect,
    ChatShared,
    ChatContact,
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