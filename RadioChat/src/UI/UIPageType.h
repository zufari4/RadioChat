#pragma once

enum class UIPageType
{
    None,
    Main,
    ChatSelect,
    Chat,
    TypingMessage,
    Statistics,
    Monitoring,
    IncomingMessage,
    Contacts,
    ContactActions,
    Logs,
    SettingsSelect,
    PropertyList,
    EditProperty,
    Ping,
};

const char* uiPageTypeToStr(UIPageType type);