#pragma once

#include "KeyboardSettings.h"
#include "WiFiSettings.h"
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
    KeyboardSettings keyboard();
    WiFiSettings wifi();
    DisplaySettings display();
    UISettings ui();
    RadioSettings radio();
    LedSettings led();
    FlashSettings flash();
    
private:
    const std::string& filename_;
};