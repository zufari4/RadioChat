#pragma once

#include "UISettings.h"
#include "UIPageType.h"
#include "../Settings/Property.h"
#include <functional>
#include <memory>

class Display;
class Settings;
class Battery;
class Radio;
class ContactsManager;
class UIPageBase;
class ChatManager;

struct UIContext
{
    using SetCurrentPageFunc = std::function<void(UIPageType)>;
    using ShowPageTypingMessageFunc = std::function<void(uint16_t)>;
    using ShowPageChatContact = std::function<void(uint16_t)>;
    using ShowPagePropertyList = std::function<void(const PropertyMap& properties)>;
    using ShowPageEditProperty = std::function<void(const Property& property)>;

    UISettings uiSettings;
    Display*   display  = nullptr;
    Settings*  settings = nullptr;
    Battery*   battery  = nullptr;
    Radio*     radio    = nullptr;
    ContactsManager* contactsManager = nullptr;
    ChatManager* chatManager = nullptr;
    uint8_t    maxLineChars  = 0;
    uint8_t    textHeight = 0;
    uint8_t    maxCountLines = 0;
    SetCurrentPageFunc setCurrentPage;
    ShowPageTypingMessageFunc showPageTypingMessage;
    ShowPagePropertyList showPagePropertyList;
    ShowPageEditProperty showPageEditProperty;
    ShowPageChatContact showPageChatContact;

    UIContext()
        : uiSettings{}, display(nullptr), settings(nullptr), battery(nullptr), radio(nullptr)
        , contactsManager(nullptr), chatManager(nullptr), maxLineChars(0), textHeight(0), maxCountLines(0)
    {
    }
    UIContext(Display* _display, Settings* _settings, Battery* _battery, Radio* _radio,
        ContactsManager* _contactsManager, ChatManager* _chatManager, uint8_t _maxStrLen, uint8_t _textHeight, uint8_t _maxCountLines,
        SetCurrentPageFunc _setCurrentPage, ShowPageTypingMessageFunc _showPageTypingMessage,
        ShowPagePropertyList _showPagePropertyList, ShowPageEditProperty _showPageEditProperty, ShowPageChatContact _showPageChatContact)
        : display(_display), settings(_settings), battery(_battery), radio(_radio)
        , contactsManager(_contactsManager), chatManager(_chatManager), maxLineChars(_maxStrLen), textHeight(_textHeight), maxCountLines(_maxCountLines)
        , setCurrentPage(_setCurrentPage), showPageTypingMessage(_showPageTypingMessage),
        showPagePropertyList(_showPagePropertyList), showPageEditProperty(_showPageEditProperty), showPageChatContact(_showPageChatContact)
    {
    }
};