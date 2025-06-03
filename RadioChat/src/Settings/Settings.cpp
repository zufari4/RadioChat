#include "Settings.h"
#include "../Logger/Logger.h"
#include "SettingsReader.h"
#include "SettingsWriter.h"
#include "../Utils.h"
#include "../Keyboard/Language.h"
#include "../Configuration.h"
#include "../Radio/Lora.h"
#include "../Flash/Flash.h"
#include <HardwareSerial.h>

Settings::Settings() = default;
Settings::~Settings() = default;

void Settings::init(std::string_view filename)
{
    filename_ = filename;

    if (!SettingsReader::isValid(filename_)) {
        LOG_INF("Settings file %s not found or bad", filename_.data());
        saveDefaultSettings();
    }
}

PropertyMap Settings::logger()
{
    LOG_INF("Loading logger settings");
    auto defaultProps = getLogDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::esp()
{
    LOG_INF("Loading ESP settings");
    auto defaultProps = getEspDefaultProperties();
    return read(defaultProps);  
}

PropertyMap Settings::keyboard()
{
    LOG_INF("Loading keyboard settings");
    auto defaultProps = getKeyboardDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::display()
{
    LOG_INF("Loading display settings");
    auto defaultProps = getDisplayDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::ui()
{
    LOG_INF("Loading UI settings");
    auto defaultProps = getUiDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::radio()
{
    LOG_INF("Loading radio settings");
    auto defaultProps = getRadioDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::led()
{
    LOG_INF("Loading led indicator settings");
    auto defaultProps = getLedDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::sound()
{
    LOG_INF("Loading sound settings");
    auto defaultProps = getSoundDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::battery()
{
    LOG_INF("Loading battery settings");
    auto defaultProps = getBatteryDefaultProperties();
    return read(defaultProps);
}

PropertyMap Settings::contacts()
{
    LOG_INF("Loading contacts settings");
    auto defaultProps = getContactsDefaultProperties();
    return read(defaultProps);
}

void Settings::setProp(SettingsWriter& opt, const Property& option)
{
    switch (option.valueType)
    {
    case ValueType::Int:
    case ValueType::Enum:
        opt.set_i(option.name, std::stoll(option.value));
        break;
    case ValueType::Float:
        opt.set_f(option.name, std::stof(option.value));
        break;
    case ValueType::String:
        opt.set_s(option.name, option.value);
        break;
    case ValueType::Bool:
        opt.set_b(option.name, option.value == "1");
        break;
    default:
        LOG_ERR("Unknown value type %d for property %s", (int)option.valueType, option.name.c_str());
        break;
    }
}

PropertyMap Settings::read(const std::vector<Property>& defaultProps)
{
    PropertyMap res;
    SettingsReader opt(filename_);
    opt.setSection(propSectionToStr(defaultProps.front().section));

    for (const auto& defProp : defaultProps) {
        Property p = defProp;

        switch (defProp.valueType)
        {
        case ValueType::Int:
        case ValueType::Enum:
            p.value = std::to_string(opt.get_i(defProp.name, std::stoll(defProp.value)));
            break;
        case ValueType::Float:
            p.value = std::to_string(opt.get_f(defProp.name, std::stof(defProp.value)));
            break;
        case ValueType::String:
            p.value = opt.get_s(defProp.name, defProp.value);
            break;
        case ValueType::Bool:
            p.value = opt.get_b(defProp.name, defProp.value == "1") ? "1" : "0";
            break;
        default:
            LOG_ERR("Unknown value type %d", (int)defProp.valueType);
            p.value = defProp.value;
            break;
        }
        res.insert({ defProp.type, p });
        LOG_DBG("Property %s: %s", defProp.name.data(), p.value.data());
    }
    return res;
}

float Settings::get_f(PropertyType type, const PropertyMap& props)
{
    const auto it = props.find(type);
    if (it == props.end()) {
        LOG_ERR("Property %d not found in settings", (int)type);
        return 0.0f;
    }
    if (it->second.valueType != ValueType::Float) {
        LOG_ERR("Property '%s' is not a float type", it->second.name.c_str());
    }
    return std::stof(it->second.value);
}

int64_t Settings::get_i(PropertyType type, const PropertyMap& props)
{
    const auto it = props.find(type);
    if (it == props.end()) {
        LOG_ERR("Property %d not found in settings", (int)type);
        return 0;
    }
    if (it->second.valueType != ValueType::Int && it->second.valueType != ValueType::Enum) {
        LOG_ERR("Property '%s' is not an int type", it->second.name.c_str());
    }
    return std::stoll(it->second.value);
}

std::string Settings::get_s(PropertyType type, const PropertyMap& props)
{
    const auto it = props.find(type);
    if (it == props.end()) {
        LOG_ERR("Property %d not found in settings", (int)type);
        return "";
    }
    if (it->second.valueType != ValueType::String) {
        LOG_ERR("Property '%s' is not a string type", it->second.name.c_str());
    }
    return it->second.value;
}

bool Settings::get_b(PropertyType type, const PropertyMap& props)
{
    const auto it = props.find(type);
    if (it == props.end()) {
        LOG_ERR("Property %d not found in settings", (int)type);
        return false;
    }
    if (it->second.valueType != ValueType::Bool) {
        LOG_ERR("Property '%s' is not a bool type", it->second.name.c_str());
    }
    return it->second.value == "1";
}

std::vector<Property> Settings::getLogDefaultProperties()
{
    std::vector<Property> props;
#if DEBUG_MODE == 1
    props.emplace_back(Property{ eLogTraceLevel, PropertySection::Log, "Уровень", "LogTraceLevel", "3", ValueType::Enum, getLogTraceLevelOptions() });
    props.emplace_back(Property{ eLogToSerial, PropertySection::Log, "Лог в порт", "LogToSerial", "1", ValueType::Bool, {} });
    props.emplace_back(Property{ eLogToFile, PropertySection::Log, "Лог в файл", "LogToFile", "0", ValueType::Bool, {} });
#else
    props.emplace_back(Property{ eLogTraceLevel, PropertySection::Log, "Уровень", "LogTraceLevel", "1", ValueType::Enum, getLogTraceLevelOptions() });
    props.emplace_back(Property{ eLogToSerial, PropertySection::Log, "Лог в порт", "LogToSerial", "0", ValueType::Bool, {} });
    props.emplace_back(Property{ eLogToFile, PropertySection::Log, "Лог в файл", "LogToFile", "1", ValueType::Bool, {} });
#endif // DEBUG
    props.emplace_back(Property{ eLogPath, PropertySection::Log, "Путь к логам", "LogPath", "logs", ValueType::String, {} });
    props.emplace_back(Property{ eLogMaxCountLogs, PropertySection::Log, "Макс файлов", "LogMaxCountLogs", "10", ValueType::Int, {} });
    props.emplace_back(Property{ eLogMaxCountLines, PropertySection::Log, "Макс строк", "LogMaxCountLines", "2000", ValueType::Int, {} });
    props.emplace_back(Property{ eLogMaxMessageSize, PropertySection::Log, "Макс длина", "LogMaxMessageSize", "256", ValueType::Int, {} });
    return props;
}

std::vector<Property> Settings::getEspDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eEspBluetoothEnable, PropertySection::Esp, "Bluetooth", "EnableBluetooth", "0", ValueType::Bool, {} });
    props.emplace_back(Property{ eEspWiFiEnable, PropertySection::Esp, "WiFi", "EnableWiFi", "0", ValueType::Bool, {} });
    props.emplace_back(Property{ eEspWiFiSSID, PropertySection::Esp, "SSID", "WiFiSSID", "RadioChat", ValueType::String, {} });
    props.emplace_back(Property{ eEspWiFiPassword, PropertySection::Esp, "Пароль", "WiFiPassword", "password", ValueType::String, {} });
    return props;
}

std::vector<Property> Settings::getKeyboardDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eKeyboardPinShLd, PropertySection::Keyboard, "Пин SH_LD", "PinSH_LD", "16", ValueType::Int, {} });
    props.emplace_back(Property{ eKeyboardPinQh, PropertySection::Keyboard, "Пин QH", "PinQH", "13", ValueType::Int, {} });
    props.emplace_back(Property{ eKeyboardPinClk, PropertySection::Keyboard, "Пин CLK", "PinCLK", "14", ValueType::Int, {} });
    props.emplace_back(Property{ eKeyboardLang, PropertySection::Keyboard, "Язык", "Language", std::to_string((int)Language::Russian), ValueType::Enum, getLangOptions() });
    props.emplace_back(Property{ eKeyboardMaxKeyNum, PropertySection::Keyboard, "Макс клавиш", "MaxCountKeys", "40", ValueType::Int, {} });
    props.emplace_back(Property{ eKeyboardFnKey, PropertySection::Keyboard, "Fn клавиша", "FnKeyCode", "2", ValueType::Int, {} });
    props.emplace_back(Property{ eKeyboardEnterKey, PropertySection::Keyboard, "Enter клавиша", "EnterKeyCode", "38", ValueType::Int, {}});
    props.emplace_back(Property{ eKeyboardCountRegisters, PropertySection::Keyboard, "Количество регистров", "CountRegisters", "5", ValueType::Int, {}});
    return props;

}

std::vector<Property> Settings::getDisplayDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eDisplayPinRS, PropertySection::Display, "Пин RS", "PinRS", "2", ValueType::Int, {} });
    props.emplace_back(Property{ eDisplayPinRW, PropertySection::Display, "Пин RW", "PinRW", "15", ValueType::Int, {} });
    props.emplace_back(Property{ eDisplayPinE, PropertySection::Display, "Пин E", "PinE", "4", ValueType::Int, {} });
    props.emplace_back(Property{ eDisplayPinRST, PropertySection::Display, "Пин RST", "PinRST", "255", ValueType::Int, {} });
    props.emplace_back(Property{ eDisplayPinBLA, PropertySection::Display, "Пин BLA", "PinBLA", "17", ValueType::Int, {} });
    props.emplace_back(Property{ eDisplayBrightnessLevel, PropertySection::Display, "Яркость", "BrightnessLevel", "255", ValueType::Int, {} });
    return props;

}

std::vector<Property> Settings::getRadioDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eRadioPinAUX, PropertySection::Radio, "Пин AUX", "PinAUX", "32", ValueType::Int, {} });
    props.emplace_back(Property{ eRadioPinM0, PropertySection::Radio, "Пин M0", "PinM0", "27", ValueType::Int, {} });
    props.emplace_back(Property{ eRadioPinM1, PropertySection::Radio, "Пин M1", "PinM1", "26", ValueType::Int, {} });
    props.emplace_back(Property{ eRadioPinRX, PropertySection::Radio, "Пин RX", "PinRX", "25", ValueType::Int, {} });
    props.emplace_back(Property{ eRadioPinTX, PropertySection::Radio, "Пин TX", "PinTX", "33", ValueType::Int, {} });
    props.emplace_back(Property{ eRadioChannel, PropertySection::Radio, "Канал", "Channel", "23", ValueType::Int, {} });
    props.emplace_back(Property{ eRadioSelfAddress, PropertySection::Radio, "Адрес", "SelfAddress", std::to_string(DEVICE_ADDRESS), ValueType::Int, {} });
    props.emplace_back(Property{ eRadioSubPacketSize, PropertySection::Radio, "Размер под-пакета", "SubPacketSize", std::to_string((int)Lora::SUB_PACKET_SETTING::e240), ValueType::Enum, getRadioSubPacketSizeOptions() });
    props.emplace_back(Property{ eRadioAirRate, PropertySection::Radio, "Скорость передачи", "AirRate", std::to_string((int)Lora::AIR_DATA_RATE::e03), ValueType::Enum, getRadioAirRateOptions() });
    props.emplace_back(Property{ eRadioUARTBaudrate, PropertySection::Radio, "Скорость UART", "UARTBaudrate", "9600", ValueType::Enum, getRadioUARTBaudrateOptions() });
    props.emplace_back(Property{ eRadioUARTParity, PropertySection::Radio, "Паритет UART", "UARTParity", std::to_string((int)SERIAL_8N1), ValueType::Enum, getRadioUARTParityOptions()});
    props.emplace_back(Property{ eRadioUARTTimeoutMs, PropertySection::Radio, "Таймаут UART", "UARTTimeoutMs", "1000", ValueType::Int, {} });
    return props;
}

std::vector<Property> Settings::getSoundDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eSoundPinIO, PropertySection::Sound, "Пин IO", "PinIO", "22", ValueType::Int, {} });
    props.emplace_back(Property{ eSoundTempo, PropertySection::Sound, "Темп", "Tempo", "108", ValueType::Int, {} });
    props.emplace_back(Property{ eSoundEnable, PropertySection::Sound, "Включить", "Enable", "1", ValueType::Bool, {} });
    props.emplace_back(Property{ eSoundEnablePlayOnPowerOn, PropertySection::Sound, "Звук включения", "PlayPowerOn", "0", ValueType::Bool, {} });
    return props;
}

std::vector<Property> Settings::getLedDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eLedPinOn, PropertySection::Led, "Пин On", "PinOn", "21", ValueType::Int, {} });
    props.emplace_back(Property{ eLedInterval, PropertySection::Led, "Интервал", "Interval", "1000", ValueType::Int, {} });
    return props;

}

std::vector<Property> Settings::getBatteryDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eBatteryPinVoltage, PropertySection::Battery, "Пин Voltage", "PinVoltage", "35", ValueType::Int, {} });
    props.emplace_back(Property{ eBatteryMaxVoltage, PropertySection::Battery, "Макс напряжение", "MaxVoltage", "8.4", ValueType::Float, {} });
    props.emplace_back(Property{ eBatteryMaxADC, PropertySection::Battery, "Макс ADC", "MaxADC", "4095.0", ValueType::Float, {} });
    props.emplace_back(Property{ eBatteryCFactor, PropertySection::Battery, "Коэф корр", "CorrectionFactor", "-0.4", ValueType::Float, {} });
    props.emplace_back(Property{ eBatteryCheckInterval, PropertySection::Battery, "Интервал проверки", "CheckInterval", "10000", ValueType::Int, {} });
    return props;
}

std::vector<Property> Settings::getContactsDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eContactsPath, PropertySection::Contacts, "Путь", "Path", "contacts", ValueType::String, {} });
    props.emplace_back(Property{ eContactsFilename, PropertySection::Contacts, "Имя файла", "FileName", "contacts.txt", ValueType::String, {} });
    return props;
}

std::vector<Property> Settings::getUiDefaultProperties()
{
    std::vector<Property> props;
    props.emplace_back(Property{ eUITextCarriageChar, PropertySection::UI, "Символ каретки", "CarriageChar", "|", ValueType::String, {} });
    props.emplace_back(Property{ eUITextHeight, PropertySection::UI, "Высота текста", "TextLineHeight", "9", ValueType::Int, {} });
    props.emplace_back(Property{ eUITextCarriageShowTime, PropertySection::UI, "Время показа каретки", "CarriageShowTime", "700", ValueType::Int, {} });
    return props;
}

EnumOption Settings::getLogTraceLevelOptions()
{
    EnumOption res;
    res.push_back(EnumOptionItem{ traceLvlToStr(LogTraceLevel::None).data(), std::to_string((int)LogTraceLevel::None) });
    res.push_back(EnumOptionItem{ traceLvlToStr(LogTraceLevel::Error).data(), std::to_string((int)LogTraceLevel::Error) });
    res.push_back(EnumOptionItem{ traceLvlToStr(LogTraceLevel::Info).data(), std::to_string((int)LogTraceLevel::Info) });
    res.push_back(EnumOptionItem{ traceLvlToStr(LogTraceLevel::Debug).data(), std::to_string((int)LogTraceLevel::Debug) });
    return res;
}

EnumOption Settings::getLangOptions()
{
    EnumOption res;
    res.push_back(EnumOptionItem{ "Русский", std::to_string((int)Language::Russian) });
    res.push_back(EnumOptionItem{ "English", std::to_string((int)Language::English) });
    return res;
}

EnumOption Settings::getRadioAirRateOptions()
{
    EnumOption res;
    res.push_back(EnumOptionItem{ "0.3k", std::to_string((int)Lora::AIR_DATA_RATE::e03) });
    res.push_back(EnumOptionItem{ "1.2k", std::to_string((int)Lora::AIR_DATA_RATE::e12) });
    res.push_back(EnumOptionItem{ "2.4k", std::to_string((int)Lora::AIR_DATA_RATE::e24) });
    res.push_back(EnumOptionItem{ "4.8k", std::to_string((int)Lora::AIR_DATA_RATE::e48) });
    res.push_back(EnumOptionItem{ "9.6k", std::to_string((int)Lora::AIR_DATA_RATE::e96) });
    res.push_back(EnumOptionItem{ "19.2k", std::to_string((int)Lora::AIR_DATA_RATE::e192) });
    res.push_back(EnumOptionItem{ "38.4k", std::to_string((int)Lora::AIR_DATA_RATE::e384) });
    res.push_back(EnumOptionItem{ "62.5k", std::to_string((int)Lora::AIR_DATA_RATE::e625) });
    return res;
}

EnumOption Settings::getRadioUARTParityOptions()
{
    EnumOption res;
    res.push_back(EnumOptionItem{ "00 8N1", std::to_string((int)SERIAL_8N1) });
    res.push_back(EnumOptionItem{ "01 8O1", std::to_string((int)SERIAL_8O1) });
    res.push_back(EnumOptionItem{ "10 8E1", std::to_string((int)SERIAL_8E1) });
    return res;
}

EnumOption Settings::getRadioSubPacketSizeOptions()
{
    EnumOption res;
    res.push_back(EnumOptionItem{ "32b", std::to_string((int)Lora::SUB_PACKET_SETTING::e032) });
    res.push_back(EnumOptionItem{ "64b", std::to_string((int)Lora::SUB_PACKET_SETTING::e064) });
    res.push_back(EnumOptionItem{ "128b", std::to_string((int)Lora::SUB_PACKET_SETTING::e128) });
    res.push_back(EnumOptionItem{ "240b", std::to_string((int)Lora::SUB_PACKET_SETTING::e240) });
    return res;
}

EnumOption Settings::getRadioUARTBaudrateOptions()
{
    EnumOption res;
    res.push_back(EnumOptionItem{ "1200bps", "1200" });
    res.push_back(EnumOptionItem{ "2400bps", "2400" });
    res.push_back(EnumOptionItem{ "4800bps", "4800" });
    res.push_back(EnumOptionItem{ "9600bps", "9600" });
    res.push_back(EnumOptionItem{ "19200bps", "19200" });
    res.push_back(EnumOptionItem{ "38400bps", "38400" });
    res.push_back(EnumOptionItem{ "57600bps", "57600" });
    res.push_back(EnumOptionItem{ "115200bps", "115200" });
    return res;
}

bool Settings::setProperty(const Property& prop)
{
    LOG_INF("Set property %s to %s", prop.name.data(), prop.value.data());
    if (!validatePropertyValue(prop)) {
        return false;
    }
    SettingsWriter opt(filename_);
    opt.setSection(propSectionToStr(prop.section));
    setProp(opt, prop);
    return opt.save();
}

bool Settings::validatePropertyValue(const Property& prop) const
{
    switch (prop.valueType) {
    case ValueType::Int:
    case ValueType::Enum:
        try {
            std::stoll(prop.value);
        } catch (...) {
            LOG_ERR("Invalid int value for property %s: %s", prop.name.c_str(), prop.value.c_str());
            return false;
        }
        break;
    case ValueType::Float:
        try {
            std::stof(prop.value);
        } catch (...) {
            LOG_ERR("Invalid float value for property %s: %s", prop.name.c_str(), prop.value.c_str());
            return false;
        }
        break;
    case ValueType::String:
        // No validation needed for string
        break;
    case ValueType::Bool:
        if (prop.value != "0" && prop.value != "1") {
            LOG_ERR("Invalid bool value for property %s: %s", prop.name.c_str(), prop.value.c_str());
            return false;
        }
        break;
    default:
        LOG_ERR("Unknown value type %d for property %s", static_cast<int>(prop.valueType), prop.name.c_str());
        return false;
    }
    return true;
}

void Settings::saveDefaultSettings()
{
    LOG_DBG("Save default settings");
    FLASH.remove(filename_);

    SettingsWriter opt(filename_);
    {
        auto props = getLogDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getEspDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getKeyboardDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getDisplayDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getRadioDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getLedDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getSoundDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getBatteryDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    {
        auto props = getContactsDefaultProperties();
        opt.setSection(propSectionToStr(props.front().section));
        for (const auto& prop : props) {
            setProp(opt, prop);
        }
    }
    bool res = opt.save();
    LOG_DBG("Save default settings: %s", res ? "success" : "fail");
}
