#include "Settings.h"
#include "Logger/Logger.h"
#include "IniFile.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

void Settings::init(const std::string& filename)
{
    filename_ = filename;
}

LoggerSettings Settings::logger()
{
    LOG_INF("Loading logger settings");
    
    LoggerSettings res;
    Ini_file ini;
    std::string section("Logger");

    res.level           = (LogTraceLevel)ini.Get_value(section, "level", (int)res.level);
    res.logToSerial     = ini.Get_value(section, "Log to serial", res.logToSerial);
    res.logToFile       = ini.Get_value(section, "Log to file", res.logToFile);
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
    Ini_file ini;
    std::string section("ESP");

    res.bluethoose.enable = ini.Get_value(section, "Enable Bluetooth", res.bluethoose.enable);
    res.wifi.enable = ini.Get_value(section, "Enable Wi-Fi", res.wifi.enable);
    res.wifi.ssid = ini.Get_value(section, "Wi-Fi SSID", res.wifi.ssid);
    res.wifi.pass = ini.Get_value(section, "Wi-Fi password", res.wifi.pass);

    return res;   
}

KeyboardSettings Settings::keyboard()
{
    LOG_INF("Loading keyboard settings");

    KeyboardSettings res;
    Ini_file ini;
    std::string section("Keyboard");
    
    res.countRegisters = ini.Get_value(section, "Count registers", res.countRegisters);
    res.enterKey = ini.Get_value(section, "Enter key code", res.enterKey);
    res.fnKey    = ini.Get_value(section, "FN key code", res.fnKey);
    res.lang     = (Language)ini.Get_value(section, "Language", (int)res.lang);
    res.maxKeyNum= ini.Get_value(section, "Max count keys", res.maxKeyNum);
    res.pins.CLK = ini.Get_value(section, "Pin CLK", res.pins.CLK);
    res.pins.QH  = ini.Get_value(section, "Pin QH", res.pins.QH);
    res.pins.SH_LD = ini.Get_value(section, "Pin SH/LD", res.pins.SH_LD);

    return res;
}

DisplaySettings Settings::display()
{
    LOG_INF("Loading display settings");

    DisplaySettings res;
    Ini_file ini;
    std::string section("Display");
    
    res.brightnessLevel = ini.Get_value(section, "Brightness", res.brightnessLevel);
    res.pins.RS  = ini.Get_value(section, "Pin RS", res.pins.RS);
    res.pins.R_W = ini.Get_value(section, "Pin R/W", res.pins.R_W);
    res.pins.E = ini.Get_value(section, "Pin E", res.pins.E);
    res.pins.RST = ini.Get_value(section, "Pin RST", res.pins.RST);
    res.pins.BLA = ini.Get_value(section, "Pin BLA", res.pins.BLA);

    return res;
}

UISettings Settings::ui()
{
    LOG_INF("Loading UI settings");

    UISettings res;
    Ini_file ini;
    std::string section("UI");
    
    res.carriageChar = (int)ini.Get_value(section, "Carriage char", (int)res.carriageChar);
    res.carriageShowTime  = ini.Get_value(section, "Carriage show time", res.carriageShowTime);
    res.textHeight = ini.Get_value(section, "Text line height", res.textHeight);

    return res;
}

RadioSettings Settings::radio()
{
    LOG_INF("Loading radio settings");

    RadioSettings res;
    Ini_file ini;
    std::string section("Radio");
    
    res.selfAddress  = ini.Get_value(section, "Address", res.selfAddress);
    res.channel  = ini.Get_value(section, "Channel", res.channel);
    res.pins.AUX = ini.Get_value(section, "Pin AUX", res.pins.AUX);
    res.pins.M0  = ini.Get_value(section, "Pin M0", res.pins.M0);
    res.pins.M1  = ini.Get_value(section, "Pin M1", res.pins.M1);
    res.pins.RX  = ini.Get_value(section, "Pin RX", res.pins.RX);
    res.pins.TX  = ini.Get_value(section, "Pin TX", res.pins.TX);
    res.uart.baudrate  = ini.Get_value(section, "Uart baudrate", res.uart.baudrate);
    res.uart.parity    = ini.Get_value(section, "Uart parity", res.uart.parity);
    res.uart.timeoutMs = ini.Get_value(section, "Uart timeout ms", res.uart.timeoutMs);

    return res;
}

LedSettings Settings::led()
{
    LOG_INF("Loading led indicator settings");

    LedSettings res;
    Ini_file ini;
    std::string section("Led indicator");
    
    res.interval = ini.Get_value(section, "Blink interval", res.interval);
    res.pins.on = ini.Get_value(section, "Pin on", res.pins.on);

    return res;
}

SoundSettings Settings::sound()
{
    LOG_INF("Loading sound settings");
    
    SoundSettings res;
    Ini_file ini;
    std::string section("Sound");
    
    res.tempo = ini.Get_value(section, "Tempo", res.tempo);
    res.pins.io = ini.Get_value(section, "Pin IO", res.pins.io);

    return res;  
}

BatterySettings Settings::battery()
{
    LOG_INF("Loading battery settings");
    
    BatterySettings res;
    Ini_file ini;
    std::string section("Battery");
    
    res.maxADC = ini.Get_value(section, "Max ADC", res.maxADC);
    res.maxBatteryVoltage = ini.Get_value(section, "Max battery voltage", res.maxBatteryVoltage);
    res.pinVotage = ini.Get_value(section, "Pin votage", res.pinVotage);
    res.checkInterval = ini.Get_value(section, "Check interval", res.checkInterval);
    res.cFactor = ini.Get_value(section, "Correction factor", res.cFactor);

    return res;  
}

ContactsSettings Settings::contacts()
{
    LOG_INF("Loading contacts");
    ContactsSettings res;
    Ini_file ini;
    std::string section("Contacts");
    
    res.path = ini.Get_value(section, "Path", res.path);
    res.filename = ini.Get_value(section, "File name", res.filename);

    return res;
}
