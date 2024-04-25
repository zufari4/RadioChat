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
    LOG_INF("Loading logger settings\n");
    LoggerSettings res;
    return res;
}

EspSettings Settings::esp()
{
    LOG_INF("Loading ESP settings\n");
    EspSettings res;
    return res;   
}

KeyboardSettings Settings::keyboard()
{
    LOG_INF("Loading keyboard settings\n");
    KeyboardSettings res;
    return res;
}

DisplaySettings Settings::display()
{
    LOG_INF("Loading display settings\n");
    DisplaySettings res;
    return res;
}

UISettings Settings::ui()
{
    LOG_INF("Loading UI settings\n");
    UISettings res;
    return res;
}

RadioSettings Settings::radio()
{
    LOG_INF("Loading radio settings\n");
    RadioSettings res;
    return res;
}

LedSettings Settings::led()
{
    LOG_INF("Loading led indicator settings\n");
    LedSettings res;
    return res;
}
