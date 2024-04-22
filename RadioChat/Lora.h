#pragma once

#include <stdint.h>
#include <string>

#define BROADCAST_ADDRESS 0xFFFF

namespace Lora
{
enum class PROGRAM_COMMAND: uint8_t
{
    WRITE_CFG_PWR_DWN_SAVE      = 0xC0,
    READ_CONFIGURATION          = 0xC1,
    WRITE_CFG_PWR_DWN_LOSE      = 0xC2,
    WRONG_FORMAT                = 0xFF,
    RETURNED_COMMAND            = 0xC1,
    SPECIAL_WIFI_CONF_COMMAND   = 0xCF,
    UNDEF = 0
};

enum class REGISTER_ADDRESS: uint8_t 
{
    REG_ADDRESS_CFG            = 0x00,
    REG_ADDRESS_SPED        = 0x03,
    REG_ADDRESS_TRANS_MODE  = 0x04,
    REG_ADDRESS_CHANNEL     = 0x05,
    REG_ADDRESS_OPTION      = 0x06,
    REG_ADDRESS_CRYPT       = 0x07,
    REG_ADDRESS_PID         = 0x80,
};

enum class PACKET_LENGHT: uint8_t
{
    PL_CONFIGURATION    = 0x09, 
    PL_SPED             = 0x01,
    PL_OPTION           = 0x01,
    PL_TRANSMISSION_MODE= 0x01,
    PL_CHANNEL          = 0x01,
    PL_CRYPT            = 0x02,
    PL_PID              = 7,
    PL_UNDEF = 0
};

#pragma pack(push, 1)
struct Speed 
{
    uint8_t airDataRate  :3; //bit 0-2
    uint8_t uartParity   :2; //bit 3-4
    uint8_t uartBaudRate :3; //bit 5-7
};

struct TransmissionMode 
{
    uint8_t WORPeriod :3; //bit 2,1,0
    uint8_t WORTransceiverControl :1; //bit 3
    uint8_t enableLBT :1; //bit 4
    uint8_t enableRepeater :1; //bit 5
    uint8_t fixedTransmission :1; //bit 6
    uint8_t enableRSSI :1; //bit 7
};

struct Option 
{
    uint8_t transmissionPower :2; //bit 0-1
    uint8_t reserve :3; //bit 2-3
    uint8_t RSSIAmbientNoise :1; //bit 5
    uint8_t subPacketSetting :2; //bit 6-7
};

struct Crypt 
{
    uint8_t CRYPT_H = 0;
    uint8_t CRYPT_L = 0;
};

struct Configuration 
{
    PROGRAM_COMMAND  command = PROGRAM_COMMAND::UNDEF;
    REGISTER_ADDRESS address = REGISTER_ADDRESS::REG_ADDRESS_CFG;
    PACKET_LENGHT    length  = PACKET_LENGHT::PL_UNDEF;

    uint8_t addh  = 0;
    uint8_t addl  = 0;
    uint8_t netid = 0;

    Speed  speed  = {0};
    Option option = {0};

    uint8_t chan  = 0;

    TransmissionMode transMode = {0};

    Crypt crypt = {0};
};

struct ModuleInformation 
{
    PROGRAM_COMMAND  command = PROGRAM_COMMAND::UNDEF;
    REGISTER_ADDRESS address = REGISTER_ADDRESS::REG_ADDRESS_CFG;
    PACKET_LENGHT    length  = PACKET_LENGHT::PL_UNDEF;

    uint8_t reserved[3] = {0,0,0};

    uint8_t model = 0;
    uint8_t version = 0;
    uint8_t features = 0;
    uint8_t end = 0;
};
#pragma pack(pop)

enum class Mode: uint8_t
{
    Undefined,
    Transfer,
    Configuration,
    WOR,
    Sleep,
};

enum class UART_PARITY: uint8_t
{
    MODE_00_8N1 = 0b00,
    MODE_01_8O1 = 0b01,
    MODE_10_8E1 = 0b10,
    MODE_11_8N1 = 0b11
};

enum class UART_BPS_TYPE: uint8_t
{
    e1200   = 0b000,
    e2400   = 0b001,
    e4800   = 0b010,
    e9600   = 0b011,
    e19200  = 0b100,
    e38400  = 0b101,
    e57600  = 0b110,
    e115200 = 0b111
};

enum class AIR_DATA_RATE: uint8_t
{
    e03  = 0b000,
    e12  = 0b001,
    e24  = 0b010,
    e48  = 0b011,
    e96  = 0b100,
    e192 = 0b101,
    e384 = 0b110,
    e625 = 0b111
};

enum class SUB_PACKET_SETTING: uint8_t
{
    e240 = 0b00,
    e128 = 0b01,
    e064 = 0b10,
    e032 = 0b11
};

enum class RSSI_AMBIENT_NOISE_ENABLE: uint8_t
{
    eENABLED  = 0b1,
    eDISABLED = 0b0
};

enum class WOR_PERIOD: uint8_t
{
    e500  = 0b000,
    e1000 = 0b001,
    e1500 = 0b010,
    e2000 = 0b011,
    e2500 = 0b100,
    e3000 = 0b101,
    e3500 = 0b110,
    e4000 = 0b111
};

enum class WOR_TRANSCEIVER_CONTROL: uint8_t
{
    eTRANSMITTER = 0b1,
    eRECEIVER    = 0b0
};

enum class LBT_ENABLE: uint8_t
{
    eENABLED  = 0b1,
    eDISABLED = 0b0
};

enum class REPEATER_MODE_ENABLE: uint8_t
{
    eENABLED  = 0b1,
    eDISABLED = 0b0
};

enum class RSSI_ENABLE: uint8_t
{
    eENABLED  = 0b1,
    eDISABLED = 0b0
};

enum class FIXED_TRANSMISSION: uint8_t
{
	eTRANSPARENT = 0b0,
	eFIXED       = 0b1
};

enum class TRANSMISSION_POWER: uint8_t
{
    e30 = 0b00,
    e27 = 0b01,
    e24 = 0b10,
    e21 = 0b11
};

const char* radio_mode_str(Mode mode);
std::string channel_str(uint8_t channel);
const char* parity_str(uint8_t parity);
const char* bps_type_str(uint8_t bpsType);
const char* air_rate_str(uint8_t airRate);
const char* sub_pack_str(uint8_t subPack);
const char* rssi_noize_str(uint8_t rssiNoize);
const char* wor_period_str(uint8_t worPeriod);
const char* wor_control_str(uint8_t worCtrl);
const char* lbt_enable_str(uint8_t lbtEnable);
const char* rssi_enable_str(uint8_t rssiEnable);
const char* repeater_enable_str(uint8_t repeaterEnable);
const char* fixed_transmiss_str(uint8_t fixedTrans);
const char* transmission_power_str(uint8_t power);
uint16_t get_address(uint8_t addrh, uint8_t addrl);
uint16_t get_address(const Configuration& cfg);
uint8_t get_addr_h(uint16_t addr);
uint8_t get_addr_l(uint16_t addr);
} // namespace Lora