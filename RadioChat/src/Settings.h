#pragma once

#include "Keyboard/KeyboardSettings.h"
#include "Esp/EspSettings.h"
#include "Display/DisplaySettings.h"
#include "UI/UISettings.h"
#include "Radio/RadioSettings.h"
#include "Led/LedSettings.h"
#include "Logger/LoggerSettings.h"
#include "Sound/SoundSettings.h"
#include "Battery/BatterySettings.h"
#include "Contacts/ContactsSettings.h"
#include <string>
#include <memory>

class Settings
{
public:
    Settings();
    ~Settings();
    void init(const std::string& filename);

    LoggerSettings logger();
    EspSettings esp();
    KeyboardSettings keyboard();
    DisplaySettings display();
    UISettings ui();
    RadioSettings radio();
    LedSettings led();
    SoundSettings sound();
    BatterySettings battery();
    ContactsSettings contacts();

    bool save(const LoggerSettings&);
    bool save(const EspSettings&);
    bool save(const KeyboardSettings&);
    bool save(const DisplaySettings&);
    bool save(const UISettings&);
    bool save(const RadioSettings&);
    bool save(const LedSettings&);
    bool save(const SoundSettings&);
    bool save(const BatterySettings&);
    bool save(const ContactsSettings&);

private:
    std::string filename_;
};