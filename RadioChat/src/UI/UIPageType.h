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
    SettingsLogger,
    SettingsEsp,
    SettingsKeyboard,
    SettingsDisplay,
    SettingsUI,
    SettingsRadio,
    SettingsLed,
    SettingsSound,
    SettingsBattery,
    SettingsContacts,
};

const char* uiPageTypeToStr(UIPageType type);