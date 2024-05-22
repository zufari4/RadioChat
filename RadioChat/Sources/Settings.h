#pragma once

#include "Keyboard/KeyboardSettings.h"
#include "Esp/EspSettings.h"
#include "Display/DisplaySettings.h"
#include "UI/UISettings.h"
#include "Radio/RadioSettings.h"
#include "Led/LedSettings.h"
#include "Logger/LoggerSettings.h"
#include "Sound/SoundSettings.h"
#include <string>

class Flash;

class Settings
{
public:
    Settings();
    ~Settings();
    void init(const std::string& filename, Flash* flash);

    LoggerSettings logger();
    EspSettings esp();
    KeyboardSettings keyboard();
    DisplaySettings display();
    UISettings ui();
    RadioSettings radio();
    LedSettings led();
    SoundSettings sound();
    
private:
    std::string filename_;
    Flash* flash_;
};