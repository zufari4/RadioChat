#pragma once

#include "KeyboardSettings.h"
#include "EspSettings.h"
#include "DisplaySettings.h"
#include "UISettings.h"
#include "RadioSettings.h"
#include "LedSettings.h"
#include "LoggerSettings.h"
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

private:
    std::string filename_;
    Flash* flash_;
};