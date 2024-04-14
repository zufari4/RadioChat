#pragma once

#include "KeyboardSettings.h"
#include "WiFiSettings.h"
#include "DisplaySettings.h"
#include "UISettings.h"
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
    
private:
    const std::string& filename_;
};