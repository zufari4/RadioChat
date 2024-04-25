#include "Settings.h"
#include "Logger.h"

Settings::Settings(const std::string& filename)
    : filename_(filename)
{
}

Settings::~Settings()
{
}

KeyboardSettings Settings::keyboard()
{
    LOG("Loading keyboard settings\n");
    KeyboardSettings res;
    return res;
}

WiFiSettings Settings::wifi()
{
    LOG("Loading Wi-Fi settings\n");
    WiFiSettings res;
    return res;
}

DisplaySettings Settings::display()
{
    LOG("Loading display settings\n");
    DisplaySettings res;
    return res;
}

UISettings Settings::ui()
{
    LOG("Loading UI settings\n");
    UISettings res;
    return res;
}

RadioSettings Settings::radio()
{
    LOG("Loading radio settings\n");
    RadioSettings res;
    return res;
}

LedSettings Settings::led()
{
    LOG("Loading led indicator settings\n");
    LedSettings res;
    return res;
}

FlashSettings Settings::flash()
{
    LOG("Loading flash settings\n");
    FlashSettings res;
    return res;
}