#include "Lora.h"

namespace Lora
{

#define INVALID "Invalid"
#define ENABLED_STR "Enabled"
#define DISABLED_STR "Disabled"

const char* radio_mode_str(Mode mode)
{
    switch (mode)
    {
    case Mode::Undefined    : return "Undefined";
    case Mode::Transfer     : return "Transfer";
    case Mode::WOR          : return "WOR";
    case Mode::Configuration: return "Configuration";
    case Mode::Sleep        : return "Sleep";
    default                 : return INVALID;
    }
}

std::string channel_str(uint8_t channel)
{
    return std::to_string(channel + 410) + "MHz";
}

const char* parity_str(uint8_t parity)
{
    switch ((UART_PARITY)parity)
    {
    case UART_PARITY::MODE_00_8N1: return "8N1"; // default
    case UART_PARITY::MODE_01_8O1: return "8O1";
    case UART_PARITY::MODE_10_8E1: return "8E1";
    case UART_PARITY::MODE_11_8N1: return "8N1";
    default: return INVALID;
    }
}

const char* bps_type_str(uint8_t bpsType)
{
    switch ((UART_BPS_TYPE)bpsType)
    {
    case UART_BPS_TYPE::e1200  : return "1200";
    case UART_BPS_TYPE::e2400  : return "2400";
    case UART_BPS_TYPE::e4800  : return "4800";
    case UART_BPS_TYPE::e9600  : return "9600";
    case UART_BPS_TYPE::e19200 : return "19200";
    case UART_BPS_TYPE::e38400 : return "38400";
    case UART_BPS_TYPE::e57600 : return "57600";
    case UART_BPS_TYPE::e115200: return "115200";
    default: return INVALID;
    }
}

const char* air_rate_str(uint8_t airRate)
{
    switch ((AIR_DATA_RATE)airRate)
    {
    case AIR_DATA_RATE::e03 : return "0.3kbps";
    case AIR_DATA_RATE::e12 : return "1.2kbps";
    case AIR_DATA_RATE::e24 : return "2.4kbps"; // default
    case AIR_DATA_RATE::e48 : return "4.8kbps";
    case AIR_DATA_RATE::e96 : return "9.6kbps";
    case AIR_DATA_RATE::e192: return "19.2kbps";
    case AIR_DATA_RATE::e384: return "38.4kbps";
    case AIR_DATA_RATE::e625: return "62.5kbps";   
    default: return INVALID;
    }
}

const char* sub_pack_str(uint8_t subPack)
{
    switch ((SUB_PACKET_SETTING)subPack)
    {
    case SUB_PACKET_SETTING::e240: return "240 bytes";
    case SUB_PACKET_SETTING::e128: return "128 bytes";
    case SUB_PACKET_SETTING::e064: return "64 bytes";
    case SUB_PACKET_SETTING::e032: return "32 bytes";
    default: return INVALID;
    }
}

const char* rssi_noize_str(uint8_t rssiNoize)
{
    switch ((RSSI_AMBIENT_NOISE_ENABLE)rssiNoize)
    {
    case RSSI_AMBIENT_NOISE_ENABLE::eENABLED : return ENABLED_STR;
    case RSSI_AMBIENT_NOISE_ENABLE::eDISABLED: return DISABLED_STR;
    default: return INVALID;
    }
}

const char* wor_period_str(uint8_t worPeriod)
{
    switch ((WOR_PERIOD)worPeriod)
    {
    case WOR_PERIOD::e500 : return "500 ms";
    case WOR_PERIOD::e1000: return "1000 ms";
    case WOR_PERIOD::e1500: return "1500 ms";
    case WOR_PERIOD::e2000: return "2000 ms";
    case WOR_PERIOD::e2500: return "2500 ms";
    case WOR_PERIOD::e3000: return "3000 ms";
    case WOR_PERIOD::e3500: return "3500 ms";
    case WOR_PERIOD::e4000: return "4000 ms";
    default: return INVALID;
    }
}

const char* wor_control_str(uint8_t worCtrl)
{
    switch ((WOR_TRANSCEIVER_CONTROL)worCtrl)
    {
    case WOR_TRANSCEIVER_CONTROL::eTRANSMITTER: return "Transmitter";
    case WOR_TRANSCEIVER_CONTROL::eRECEIVER   : return "Receiver";
    default: return INVALID;
    }
}

const char* lbt_enable_str(uint8_t lbtEnable)
{
    switch ((LBT_ENABLE)lbtEnable)
    {
    case LBT_ENABLE::eENABLED : return ENABLED_STR;
    case LBT_ENABLE::eDISABLED: return DISABLED_STR;
    default: return INVALID;
    }
}

const char* rssi_enable_str(uint8_t rssiEnable)
{
    switch ((RSSI_ENABLE)rssiEnable)
    {
    case RSSI_ENABLE::eENABLED : return ENABLED_STR;
    case RSSI_ENABLE::eDISABLED: return DISABLED_STR;
    default: return INVALID;
    }
}

const char* repeater_enable_str(uint8_t repeaterEnable)
{
    switch ((REPEATER_MODE_ENABLE)repeaterEnable)
    {
    case REPEATER_MODE_ENABLE::eENABLED : return ENABLED_STR;
    case REPEATER_MODE_ENABLE::eDISABLED: return DISABLED_STR;
    default: return INVALID;
    }
}

const char* fixed_transmiss_str(uint8_t fixedTrans)
{
    switch ((FIXED_TRANSMISSION)fixedTrans)
    {
    case FIXED_TRANSMISSION::eTRANSPARENT: return "Transparent";
    case FIXED_TRANSMISSION::eFIXED      : return "Fixed";
    default: return INVALID;
    }
}

const char* transmission_power_str(uint8_t power)
{
    switch ((TRANSMISSION_POWER)power)
    {
    case TRANSMISSION_POWER::e30: return "30 dBm";
    case TRANSMISSION_POWER::e27: return "27 dBm";
    case TRANSMISSION_POWER::e24: return "24 dBm";
    case TRANSMISSION_POWER::e21: return "21 dBm";
    default: return INVALID;
    }
}

} // namespace Lora