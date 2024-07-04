#pragma once

#include "UISettings.h"

class Display;
class Settings;
class Battery;
class ContactsManager;

struct UIContext
{
    UISettings uiSettings;
    Display*   display  = nullptr;
    Settings*  settings = nullptr;
    Battery*   battery  = nullptr;
    ContactsManager* contactsManager = nullptr;
    uint8_t    maxStrLen  = 0;
    uint8_t    textHeight = 0;
    uint8_t    maxCountLines = 0;
    UIContext() 
              : uiSettings{}, display(nullptr), settings(nullptr), battery(nullptr)
              , contactsManager(nullptr), maxStrLen(0), textHeight(0), maxCountLines(0)
              {}
    UIContext(const UISettings& _uiSettings, Display* _display, Settings* _settings, Battery* _battery, 
              ContactsManager* _contactsManager, uint8_t _maxStrLen, uint8_t _textHeight, uint8_t _maxCountLines) 
              : uiSettings(_uiSettings), display(_display), settings(_settings), battery(_battery)
              , contactsManager(_contactsManager), maxStrLen(_maxStrLen), textHeight(_textHeight), maxCountLines(_maxCountLines)
              {}
};