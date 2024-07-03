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
#include <string>


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

private:
    std::string filename_;
};