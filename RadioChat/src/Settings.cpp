#include "Settings.h"
#include "Logger/Logger.h"
#include "SettingsReader.h"
#include "SettingsWriter.h"
#include "Utils.h"

Settings::Settings() = default;
Settings::~Settings() = default;

void Settings::init(const std::string& filename)
{
    LOG_INF("Settings initializing from file %s", filename.c_str());
    filename_ = filename;

    if (!SettingsReader::isValid(filename_)) {
        LOG_INF("Settings file %s not found or bad. Create default.", filename_.c_str());
        save(LoggerSettings());
        save(EspSettings());
        save(KeyboardSettings());
        save(DisplaySettings());
        save(UISettings());
        save(RadioSettings());
        save(LedSettings());
        save(SoundSettings());
        save(BatterySettings());
        save(ContactsSettings());
    }
}

LoggerSettings Settings::logger()
{
    LOG_INF("Loading logger settings");
    
    LoggerSettings res;
    SettingsReader opt(filename_, "Logger");

    res.level           = (LogTraceLevel)opt.get_i("level", (int)res.level);
    res.logToSerial     = opt.get_b("log_to_serial", res.logToSerial);
    res.logToFile       = opt.get_b("log_to_file", res.logToFile);
    res.logPath         = opt.get_s("path", res.logPath);
    res.maxCountLines   = opt.get_u("max_count_lines", res.maxCountLines);
    res.maxCountLogs    = opt.get_u("max_count_logs", res.maxCountLogs);
    res.maxMessageSize  = opt.get_u("max_message_size", res.maxMessageSize);

    return res;
}

EspSettings Settings::esp()
{
    LOG_INF("Loading ESP settings");

    EspSettings res;
    SettingsReader opt(filename_, "ESP");

    res.bluethoose.enable = opt.get_b("enable_bluetooth", res.bluethoose.enable);
    res.wifi.enable = opt.get_b("enable_wifi", res.wifi.enable);
    res.wifi.ssid = opt.get_s("wifi_ssid", res.wifi.ssid);
    res.wifi.pass = opt.get_s("wifi_pass", res.wifi.pass);

    return res;   
}

KeyboardSettings Settings::keyboard()
{
    LOG_INF("Loading keyboard settings");

    KeyboardSettings res;
    SettingsReader opt(filename_, "Keyboard");
    
    res.countRegisters = opt.get_u("count_registers", res.countRegisters);
    res.enterKey = opt.get_u("enter_key_code", res.enterKey);
    res.fnKey    = opt.get_u("fn_key_code", res.fnKey);
    res.lang     = (Language)opt.get_u("language", (int)res.lang);
    res.maxKeyNum= opt.get_u("max_count_keys", res.maxKeyNum);
    res.pins.CLK = opt.get_u("pin_clk", res.pins.CLK);
    res.pins.QH  = opt.get_u("pin_qh", res.pins.QH);
    res.pins.SH_LD = opt.get_u("pin_sh_ld", res.pins.SH_LD);

    return res;
}

DisplaySettings Settings::display()
{
    LOG_INF("Loading display settings");

    DisplaySettings res;
    SettingsReader opt(filename_, "Display");
    
    res.brightnessLevel = opt.get_u("brightness", res.brightnessLevel);
    res.pins.RS  = opt.get_u("pin_rs", res.pins.RS);
    res.pins.R_W = opt.get_u("pin_rw", res.pins.R_W);
    res.pins.E   = opt.get_u("pin_e", res.pins.E);
    res.pins.RST = opt.get_u("pin_rst", res.pins.RST);
    res.pins.BLA = opt.get_u("pin_bla", res.pins.BLA);

    return res;
}

UISettings Settings::ui()
{
    LOG_INF("Loading UI settings");

    UISettings res;
    SettingsReader opt(filename_, "UI");
    
    res.carriageChar = opt.get_u("carriage_char", (int)res.carriageChar);
    res.carriageShowTime  = opt.get_u("carriage_show_time", res.carriageShowTime);
    res.textHeight = opt.get_u("text_line_height", res.textHeight);

    return res;
}

RadioSettings Settings::radio()
{
    LOG_INF("Loading radio settings");

    RadioSettings res;
    SettingsReader opt(filename_, "Radio");
    
    res.selfAddress  = opt.get_u("address", res.selfAddress);
    res.channel  = opt.get_u("channel", res.channel);
    res.pins.AUX = opt.get_u("pin_aux", res.pins.AUX);
    res.pins.M0  = opt.get_u("pin_m0", res.pins.M0);
    res.pins.M1  = opt.get_u("pin_m1", res.pins.M1);
    res.pins.RX  = opt.get_u("pin_rx", res.pins.RX);
    res.pins.TX  = opt.get_u("pin_tx", res.pins.TX);
    res.uart.baudrate  = opt.get_u("uart_baudrate", res.uart.baudrate);
    res.uart.parity    = opt.get_u("uart_parity", res.uart.parity);
    res.uart.timeoutMs = opt.get_u("uart_timeout_ms", res.uart.timeoutMs);
    res.subPacketSize = opt.get_u("sub_packet_size", res.subPacketSize);
    res.airRate = opt.get_u("air_rate", res.airRate);

    return res;
}

LedSettings Settings::led()
{
    LOG_INF("Loading led indicator settings");

    LedSettings res;
    SettingsReader opt(filename_, "Led");
    
    res.interval = opt.get_u("blink_interval", res.interval);
    res.pins.on = opt.get_u("pin_on", res.pins.on);

    return res;
}

SoundSettings Settings::sound()
{
    LOG_INF("Loading sound settings");
    
    SoundSettings res;
    SettingsReader opt(filename_, "Sound");
    
    res.tempo = opt.get_u("tempo", res.tempo);
    res.pins.io = opt.get_u("pin_io", res.pins.io);

    return res;  
}

BatterySettings Settings::battery()
{
    LOG_INF("Loading battery settings");
    
    BatterySettings res;
    SettingsReader opt(filename_, "Battery");
    
    res.maxADC = opt.get_f("max_adc", res.maxADC);
    res.maxBatteryVoltage = opt.get_f("max_battery_voltage", res.maxBatteryVoltage);
    res.pinVotage = opt.get_u("pin_votage", res.pinVotage);
    res.checkInterval = opt.get_u("check_interval", res.checkInterval);
    res.cFactor = opt.get_f("correction_factor", res.cFactor);

    return res;  
}

ContactsSettings Settings::contacts()
{
    LOG_INF("Loading contacts settings");
    ContactsSettings res;
    SettingsReader opt(filename_, "Contacts");
    
    res.path = opt.get_s("path", res.path);
    res.filename = opt.get_s("file_name", res.filename);

    return res;
}

bool Settings::save(const LoggerSettings& option)
{
    SettingsWriter opt(filename_, "Logger");
    opt.set_i("level", (int)option.level);
    opt.set_b("log_to_serial", option.logToSerial);
    opt.set_b("log_to_file", option.logToFile);
    opt.set_s("path", option.logPath);
    opt.set_u("max_count_lines", option.maxCountLines);
    opt.set_u("max_count_logs", option.maxCountLogs);
    opt.set_u("max_message_size", option.maxMessageSize);
    return opt.save();
}

bool Settings::save(const EspSettings& option)
{
    SettingsWriter opt(filename_, "ESP");
    opt.set_b("enable_bluetooth", option.bluethoose.enable);
    opt.set_b("enable_wifi", option.wifi.enable);
    opt.set_s("wifi_ssid", option.wifi.ssid);
    opt.set_s("wifi_pass", option.wifi.pass);
    return opt.save();
}

bool Settings::save(const KeyboardSettings& option)
{
    SettingsWriter opt(filename_, "Keyboard");
    opt.set_u("count_registers", option.countRegisters);
    opt.set_u("enter_key_code", option.enterKey);
    opt.set_u("fn_key_code", option.fnKey);
    opt.set_u("language", (int)option.lang);
    opt.set_u("max_count_keys", option.maxKeyNum);
    opt.set_u("pin_clk", option.pins.CLK);
    opt.set_u("pin_qh", option.pins.QH);
    opt.set_u("pin_sh_ld", option.pins.SH_LD);
    return opt.save();
}

bool Settings::save(const DisplaySettings& option)
{
    SettingsWriter opt(filename_, "Display");
    opt.set_u("brightness", option.brightnessLevel);
    opt.set_u("pin_rs", option.pins.RS);
    opt.set_u("pin_rw", option.pins.R_W);
    opt.set_u("pin_e", option.pins.E);
    opt.set_u("pin_rst", option.pins.RST);
    opt.set_u("pin_bla", option.pins.BLA);
    return opt.save();
}

bool Settings::save(const UISettings& option)
{
    SettingsWriter opt(filename_, "UI");
    opt.set_u("carriage_char", (int)option.carriageChar);
    opt.set_u("carriage_show_time", option.carriageShowTime);
    opt.set_u("text_line_height", option.textHeight);
    return opt.save();
}

bool Settings::save(const RadioSettings& option)
{
    SettingsWriter opt(filename_, "Radio");
    opt.set_u("address", option.selfAddress);
    opt.set_u("channel", option.channel);
    opt.set_u("pin_aux", option.pins.AUX);
    opt.set_u("pin_m0", option.pins.M0);
    opt.set_u("pin_m1", option.pins.M1);
    opt.set_u("pin_rx", option.pins.RX);
    opt.set_u("pin_tx", option.pins.TX);
    opt.set_u("uart_baudrate", option.uart.baudrate);
    opt.set_u("uart_parity", option.uart.parity);
    opt.set_u("uart_timeout_ms", option.uart.timeoutMs);
    opt.set_u("sub_packet_size", option.subPacketSize);
    opt.set_u("air_rate", option.airRate);
    return opt.save();

}

bool Settings::save(const LedSettings& option)
{
    SettingsWriter opt(filename_, "Led");
    opt.set_u("blink_interval", option.interval);
    opt.set_u("pin_on", option.pins.on);
    return opt.save();
}

bool Settings::save(const SoundSettings& option)
{
    SettingsWriter opt(filename_, "Sound");
    opt.set_u("tempo", option.tempo);
    opt.set_u("pin_io", option.pins.io);
    return opt.save();
}

bool Settings::save(const BatterySettings& option)
{
    SettingsWriter opt(filename_, "Battery");
    opt.set_f("max_adc", option.maxADC);
    opt.set_f("max_battery_voltage", option.maxBatteryVoltage);
    opt.set_u("pin_votage", option.pinVotage);
    opt.set_u("check_interval", option.checkInterval);
    opt.set_f("correction_factor", option.cFactor);
    return opt.save();
}

bool Settings::save(const ContactsSettings& option)
{
    SettingsWriter opt(filename_, "Contacts");
    opt.set_s("path", option.path);
    opt.set_s("file_name", option.filename);
    return opt.save();
}
