#pragma once

#include <stdint.h>

namespace RadioDef
{
enum class PROGRAM_COMMAND: uint8_t
{
	WRITE_CFG_PWR_DWN_SAVE 	= 0xC0,
	READ_CONFIGURATION 		= 0xC1,
	WRITE_CFG_PWR_DWN_LOSE 	= 0xC2,
	WRONG_FORMAT 			= 0xFF,
	RETURNED_COMMAND 		= 0xC1,
	SPECIAL_WIFI_CONF_COMMAND = 0xCF
};

enum class REGISTER_ADDRESS: uint8_t 
{
	REG_ADDRESS_CFG	= 0x00,
	REG_ADDRESS_SPED 		= 0x03,
	REG_ADDRESS_TRANS_MODE 	= 0x04,
	REG_ADDRESS_CHANNEL 	= 0x05,
	REG_ADDRESS_OPTION	 	= 0x06,
	REG_ADDRESS_CRYPT	 	= 0x07,
	REG_ADDRESS_PID		 	= 0x80
};

enum class PACKET_LENGHT: uint8_t
{
	PL_CONFIGURATION 	= 0x09,

	PL_SPED				= 0x01,
	PL_OPTION			= 0x01,
	PL_TRANSMISSION_MODE= 0x01,
	PL_CHANNEL			= 0x01,
	PL_CRYPT			= 0x02,
	PL_PID				= 7,
    PL_UNDEF            = 0
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
	PROGRAM_COMMAND  command = PROGRAM_COMMAND::WRONG_FORMAT;
	REGISTER_ADDRESS address = REGISTER_ADDRESS::REG_ADDRESS_CFG;
	PACKET_LENGHT    length  = PACKET_LENGHT::PL_UNDEF;

	uint8_t addh  = 0;
	uint8_t addl  = 0;
	uint8_t netid = 0;

	Speed  speed  = {0};
	Option option = {0};

	uint8_t chan = 0;

	TransmissionMode transMode = {0};

	Crypt crypt = {0};
};

struct ModuleInformation 
{
	uint8_t COMMAND = 0;
	uint8_t STARTING_ADDRESS = 0;
	uint8_t LENGHT = 0;

	uint8_t reserved[3] = {0,0,0};

	uint8_t model = 0;
	uint8_t version = 0;
	uint8_t features = 0;
	uint8_t end = 0;
};
#pragma pack(pop)

enum class RadioMode 
{
    Undefined,
	Transfer,
    Configuration,
	WOR,
	Sleep,
};

const char* radio_mode_str(RadioMode mode);

} // namespace RadioDef