#include "Settings.h"
#include "Logger.h"

Settings::Settings()
    : flash_(nullptr)
{
}

Settings::~Settings()
{
}

void Settings::init(const std::string& filename, Flash* flash)
{
    filename_ = filename;
    flash_ = flash;
}

LoggerSettings Settings::logger()
{
    LOG_INF("Loading logger settings");
    LoggerSettings res;
    return res;
}

EspSettings Settings::esp()
{
    LOG_INF("Loading ESP settings");
    EspSettings res;
    return res;   
}

KeyboardSettings Settings::keyboard()
{
    LOG_INF("Loading keyboard settings");
    KeyboardSettings res;
    return res;
}

DisplaySettings Settings::display()
{
    LOG_INF("Loading display settings");
    DisplaySettings res;
    return res;
}

UISettings Settings::ui()
{
    LOG_INF("Loading UI settings");
    UISettings res;
    return res;
}

RadioSettings Settings::radio()
{
    LOG_INF("Loading radio settings");
    RadioSettings res;
    return res;
}

LedSettings Settings::led()
{
    LOG_INF("Loading led indicator settings");
    LedSettings res;
    return res;
}

SoundSettings Settings::sound()
{
    LOG_INF("Loading sound settings");
    SoundSettings res;
    return res;  
}