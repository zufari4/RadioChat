#include "Settings.h"
#include "Logger/Logger.h"
#include "IniFile.h"

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
    Ini_file ini(*flash_);
    std::string section("Logger");

    res.level           = (LogTraceLevel)ini.Get_value(section, "level", (int)res.level);
    res.logToSerial     = ini.Get_value(section, "Log to serial", res.logToSerial);
    res.logPath         = ini.Get_value(section, "path", res.logPath);
    res.maxCountLines   = ini.Get_value(section, "Max count lines", (int)res.maxCountLines);
    res.maxCountLogs    = ini.Get_value(section, "Max count logs", (int)res.maxCountLogs);
    res.maxMessageSize  = ini.Get_value(section, "Max message size", (int)res.maxMessageSize);

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