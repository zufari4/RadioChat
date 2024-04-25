#pragma once

#include "KeyboardSettings.h"
#include "EspSettings.h"
#include "DisplaySettings.h"
#include "UISettings.h"
#include "RadioSettings.h"
#include "LedSettings.h"
#include "FlashSettings.h"
#include <string>

class Settings
{
public:
    Settings(const std::string& filename);
    ~Settings();

    FlashSettings flash();
    EspSettings esp();
    KeyboardSettings keyboard();
    DisplaySettings display();
    UISettings ui();
    RadioSettings radio();
    LedSettings led();

private:
    const std::string& filename_;
};