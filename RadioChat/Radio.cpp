#include "core_esp8266_features.h"
#include "Radio.h"
#include "Logger.h"
#include <HardwareSerial.h>
#include <Arduino.h>

Radio::Radio()
    : currentMode_(RadioDef::RadioMode::Undefined)
{
}

Radio::~Radio()
{

}

bool Radio::init(const RadioSettings& settings)
{
    LOG("Initialize radio module\n");

    settings_ = settings;
    LOG("AUX %u M0 %u M1 %u\n", settings_.pins.AUX, settings_.pins.M0, settings_.pins.M1);
    LOG("channel %u address %u speed %u uartBaudrate %u\n", settings_.channel, settings_.address, settings_.speed, settings_.uartBaudrate);
    
    LOG("Setup pins\n");
    pinMode(settings_.pins.AUX, INPUT);
	pinMode(settings_.pins.M0, OUTPUT);
    pinMode(settings_.pins.M1, OUTPUT);

    LOG("Wait device is ready\n");
    if (!waitReady()) {
        return false;
    }

	//digitalWrite(settings_.pins.M0, HIGH);
	//digitalWrite(settings_.pins.M1, HIGH);
    
    LOG("Init UART\n");
    Serial1.begin(settings_.uartBaudrate);
    while (!Serial1) { } //wait for serial port to connect
    Serial1.setTimeout(settings_.uartTimeoutMs);
    
    RadioDef::Configuration cfg;
    if (!getConfiguration(cfg)) {
        traceConfig(cfg);
        return false;
    }

    return true;
}

bool Radio::getConfiguration(RadioDef::Configuration& out)
{
    LOG("Get configuration\n");
    RadioDef::RadioMode prevMode = currentMode_;
    bool res = false;

    do {
        if (setMode(RadioDef::RadioMode::Configuration)) {
            break;
        }
        if (!writeProgramCommand(RadioDef::PROGRAM_COMMAND::READ_CONFIGURATION,
                                 RadioDef::REGISTER_ADDRESS::REG_ADDRESS_CFG, 
                                 RadioDef::PACKET_LENGHT::PL_CONFIGURATION)) {
            break;
        }
        RadioDef::Configuration cfg;
        if (!receiveData(&cfg, sizeof(cfg))) {
            break;
        }
        if (RadioDef::PROGRAM_COMMAND::WRONG_FORMAT == cfg.command) {
		    LOG("Wrong format\n");
            break;
        }
        if (RadioDef::PROGRAM_COMMAND::RETURNED_COMMAND != cfg.command || 
            RadioDef::REGISTER_ADDRESS::REG_ADDRESS_CFG != cfg.address || 
            RadioDef::PACKET_LENGHT::PL_CONFIGURATION != cfg.length) {
            LOG("Head is not recognized\n");
		    break;
	    }
	} while(false);

    res = res && setMode(prevMode);
    return res;
}

bool Radio::isReady()
{
    return digitalRead(settings_.pins.AUX) == HIGH;
}

bool Radio::waitReady()
{
    const unsigned long timeout = millis() + settings_.uartTimeoutMs;
    while (!isReady() && millis() < timeout) {
        yield();
    }
    if (millis() >= timeout) {
        LOG("Timeout of wait AUX ready\n");
        return false;
    }
    else {
        delay(20); // per data sheet control after aux goes high is 2ms so delay for at least that long
    }

    return true;
}

bool Radio::setMode(RadioDef::RadioMode mode)
{
    if (mode == RadioDef::RadioMode::Undefined) {
        return true;
    }
    LOG("Set radio mode: %s\n", radio_mode_str(mode));

    // data sheet claims module needs some extra time after mode setting (2ms)
	// most of my projects uses 10 ms, but 40ms is safer
    delay(40);
    
    switch (mode)
    {
    case RadioDef::RadioMode::Transfer:
        digitalWrite(settings_.pins.M1, LOW);
        digitalWrite(settings_.pins.M0, LOW);
        break;
    case RadioDef::RadioMode::Configuration:
        digitalWrite(settings_.pins.M1, HIGH);
        digitalWrite(settings_.pins.M0, LOW);
        break;
    case RadioDef::RadioMode::WOR:
        digitalWrite(settings_.pins.M1, LOW);
        digitalWrite(settings_.pins.M0, HIGH);
        break;
    case RadioDef::RadioMode::Sleep:
        digitalWrite(settings_.pins.M1, HIGH);
        digitalWrite(settings_.pins.M0, HIGH);
        break;
    }

	// data sheet says 2ms later control is returned, let's give just a bit more time
	// these modules can take time to activate pins
	delay(40);
	bool res = waitReady();
    if (!res) {
        LOG("Can't wait status of set mode\n");
    }
    return res;
}

bool Radio::writeProgramCommand(RadioDef::PROGRAM_COMMAND cmd, RadioDef::REGISTER_ADDRESS addr, RadioDef::PACKET_LENGHT pl)
{
	uint8_t data[3] = {(uint8_t)cmd, (uint8_t)addr, (uint8_t)pl};
	uint8_t size = Serial1.write(data, 3);
    delay(50);  //need ti check
    if (size != 3) {
        LOG("Wrong size of write command: %u\n", size);
    }
	return size != 3;
}

bool Radio::receiveData(void *out, uint8_t dataSize) 
{
	uint8_t len = Serial1.readBytes((uint8_t *)out, dataSize);
	if (len != dataSize) {
		LOG("Wrong length of receive. Need %u actual %u\n", dataSize, len);
        return false;
	}
	bool res = waitReady();
    if (!res) {
        LOG("Can't wait status of read bytes\n");
    }
    return res;
}

void Radio::traceConfig(const RadioDef::Configuration& cfg) const
{
	LOG("----------------------------------------\n");

	LOG("HEAD: %02X %02X %02X\n", cfg.command, cfg.address, cfg.length);
	LOG("AddH : %02X\n", cfg.addh);  
	LOG("AddL : %02X\n", cfg.addl);  
	LOG("NetID: %02X\n\n", cfg.netid); 
	LOG("Channel: %u\n\n", cfg.chan);  //LOG(, DEC); LOG(" -> "); LOG(cfg.getChannelDescription());

	//LOG("SpeedParityBit     : "));  LOG(cfg.SPED.uartParity, BIN);LOG(" -> "); LOG(cfg.SPED.getUARTParityDescription());
	//LOG("SpeedUARTDatte     : "));  LOG(cfg.SPED.uartBaudRate, BIN);LOG(" -> "); LOG(cfg.SPED.getUARTBaudRateDescription());
	//LOG("SpeedAirDataRate   : "));  LOG(cfg.SPED.airDataRate, BIN);LOG(" -> "); LOG(cfg.SPED.getAirDataRateDescription());

	//LOG("OptionSubPacketSett: "));  LOG(cfg.OPTION.subPacketSetting, BIN);LOG(" -> "); LOG(cfg.OPTION.getSubPacketSetting());
	//LOG("OptionTranPower    : "));  LOG(cfg.OPTION.transmissionPower, BIN);LOG(" -> "); LOG(cfg.OPTION.getTransmissionPowerDescription());
	//LOG("OptionRSSIAmbientNo: "));  LOG(cfg.OPTION.RSSIAmbientNoise, BIN);LOG(" -> "); LOG(cfg.OPTION.getRSSIAmbientNoiseEnable());

	//LOG("TransModeWORPeriod : "));  LOG(cfg.TRANSMISSION_MODE.WORPeriod, BIN);LOG(" -> "); LOG(cfg.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	//LOG("TransModeTransContr: "));  LOG(cfg.TRANSMISSION_MODE.WORTransceiverControl, BIN);LOG(" -> "); LOG(cfg.TRANSMISSION_MODE.getWORTransceiverControlDescription());
	//LOG("TransModeEnableLBT : "));  LOG(cfg.TRANSMISSION_MODE.enableLBT, BIN);LOG(" -> "); LOG(cfg.TRANSMISSION_MODE.getLBTEnableByteDescription());
	//LOG("TransModeEnableRSSI: "));  LOG(cfg.TRANSMISSION_MODE.enableRSSI, BIN);LOG(" -> "); LOG(cfg.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	//LOG("TransModeEnabRepeat: "));  LOG(cfg.TRANSMISSION_MODE.enableRepeater, BIN);LOG(" -> "); LOG(cfg.TRANSMISSION_MODE.getRepeaterModeEnableByteDescription());
	//LOG("TransModeFixedTrans: "));  LOG(cfg.TRANSMISSION_MODE.fixedTransmission, BIN);LOG(" -> "); LOG(cfg.TRANSMISSION_MODE.getFixedTransmissionDescription());


	LOG("----------------------------------------\n");
}