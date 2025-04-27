#pragma once

enum class UIPageType
{
    None,
    Main,
    ChatSelect,
    TypingMessage,
    Settings,
    Statistics,
    Monitoring,
    IncomingMessage,
    Contacts,
    Logs,
};

const char* uiPageTypeToStr(UIPageType type);