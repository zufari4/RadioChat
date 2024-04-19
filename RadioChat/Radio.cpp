#include <stdint.h>
#include "Radio.h"
#include "Logger.h"
#include <HardwareSerial.h>
#include <Arduino.h>

Radio::Radio()
    : currentMode_(Lora::Mode::Undefined)
    , isInit_(false)
{
}

Radio::~Radio()
{

}

void Radio::init(const RadioSettings& settings)
{
    LOG("-- Initialize radio module --\n");

    settings_ = settings;
    LOG("RX %u TX %u AUX %u M0 %u M1 %u\n", settings_.pins.RX, settings_.pins.TX, settings_.pins.AUX, settings_.pins.M0, settings_.pins.M1);
    LOG("channel %u address %u speed %u\n", settings_.channel, settings_.address, settings_.speed);
    LOG("baudrate %u timeout %u parity %u\n", settings_.uart.baudrate, settings_.uart.timeoutMs, settings_.uart.parity);
    
    LOG("Setup pins\n");
    pinMode(settings_.pins.AUX, INPUT);
	pinMode(settings_.pins.M0, OUTPUT);
    pinMode(settings_.pins.M1, OUTPUT);

    LOG("Wait device is ready\n");
    if (!waitReady()) {
        return;
    }
    
    LOG("Init UART\n");
    Serial2.setTimeout(settings_.uart.timeoutMs);
    Serial2.begin(settings_.uart.baudrate, settings_.uart.parity, settings_.pins.TX, settings_.pins.RX);
    while (!Serial2) { } //wait for serial port to connect
    
    Lora::Configuration cfg;
    if (!getConfiguration(cfg)) {
        LOG("Can't get radio configuration\n")
        return;
    }
    traceConfig(cfg);

    isInit_ = setMode(Lora::Mode::Transfer);
}

bool Radio::isInit() const
{
    return isInit_;
}

bool Radio::getConfiguration(Lora::Configuration& out)
{
    LOG("Get configuration\n");
    Lora::Mode prevMode = currentMode_;
    bool res = false;

    do {
        if (!setMode(Lora::Mode::Configuration)) {
            break;
        }
        if (!writeProgramCommand(Lora::PROGRAM_COMMAND::READ_CONFIGURATION,
                                 Lora::REGISTER_ADDRESS::REG_ADDRESS_CFG, 
                                 Lora::PACKET_LENGHT::PL_CONFIGURATION)) {
            break;
        }
        Lora::Configuration cfg;
        if (!readData(&cfg, sizeof(cfg))) {
            break;
        }
        if (Lora::PROGRAM_COMMAND::WRONG_FORMAT == cfg.command) {
		    LOG("Wrong format\n");
            break;
        }
        if (Lora::PROGRAM_COMMAND::RETURNED_COMMAND != cfg.command || 
            Lora::REGISTER_ADDRESS::REG_ADDRESS_CFG != cfg.address || 
            Lora::PACKET_LENGHT::PL_CONFIGURATION   != cfg.length) {
            LOG("Head is not recognized\n");
		    break;
	    }
        out = cfg;
	} while(false);

    return setMode(prevMode);
}

bool Radio::isReady()
{
    return digitalRead(settings_.pins.AUX) == HIGH;
}

bool Radio::waitReady()
{
    const unsigned long timeout = millis() + settings_.uart.timeoutMs;
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

bool Radio::setMode(Lora::Mode mode)
{
    if (mode == Lora::Mode::Undefined) {
        return true;
    }
    LOG("Set radio mode: %s\n", radio_mode_str(mode));

    // data sheet claims module needs some extra time after mode setting (2ms)
	// most of my projects uses 10 ms, but 40ms is safer
    //delay(40);
    
    switch (mode)
    {
    case Lora::Mode::Transfer:
        digitalWrite(settings_.pins.M1, LOW);
        digitalWrite(settings_.pins.M0, LOW);
        break;
    case Lora::Mode::Configuration:
        digitalWrite(settings_.pins.M1, HIGH);
        digitalWrite(settings_.pins.M0, LOW);
        break;
    case Lora::Mode::WOR:
        digitalWrite(settings_.pins.M1, LOW);
        digitalWrite(settings_.pins.M0, HIGH);
        break;
    case Lora::Mode::Sleep:
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

bool Radio::writeProgramCommand(Lora::PROGRAM_COMMAND cmd, Lora::REGISTER_ADDRESS addr, Lora::PACKET_LENGHT pl)
{
	uint8_t data[3] = {(uint8_t)cmd, (uint8_t)addr, (uint8_t)pl};
    LOG("Write command --> %02X %02X %02X\n", data[0], data[1], data[2]);

	uint8_t size = Serial2.write(data, 3);
    delay(50);  //need ti check
    if (size != 3) {
        LOG("Wrong size of write command: %u\n", size);
    }
	return size == 3;
}

bool Radio::readData(void* out, uint8_t dataSize) 
{
    uint8_t* dst = (uint8_t*)out;
	uint8_t len = Serial2.readBytes(dst, dataSize);
    if (len > 0) {
        LOGX("<-- ");
        for (uint8_t i = 0; i < len; ++i) {
            LOGX("%02X ", dst[i]);
        }
        LOGX("\n");
    }
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

void Radio::traceConfig(const Lora::Configuration& cfg) const
{
	LOG("HEAD: %02X %02X %02X\n", (uint8_t)cfg.command, (uint8_t)cfg.address, (uint8_t)cfg.length);
	LOG("AddH : %02X\n", cfg.addh);  
	LOG("AddL : %02X\n", cfg.addl);  
	LOG("NetID: %02X\n", cfg.netid); 
	LOG("Channel: %s\n", Lora::channel_str(cfg.chan).c_str());
	LOG("UART parity: %s\n", Lora::parity_str(cfg.speed.uartParity));
	LOG("UART baud rate: %s\n", Lora::bps_type_str(cfg.speed.uartBaudRate));
    LOG("Air data rate: %s\n", Lora::air_rate_str(cfg.speed.airDataRate));
    LOG("Subpacket size: %s\n", Lora::sub_pack_str(cfg.option.subPacketSetting));
    LOG("Transmission power: %s\n", Lora::transmission_power_str(cfg.option.transmissionPower));
    LOG("RSSI ambient noise: %s\n", Lora::rssi_noize_str(cfg.option.RSSIAmbientNoise));
    LOG("WOR period: %s\n", Lora::wor_period_str(cfg.transMode.WORPeriod));
    LOG("WOR control: %s\n", Lora::wor_control_str(cfg.transMode.WORTransceiverControl));
    LOG("LBT: %s\n", Lora::lbt_enable_str(cfg.transMode.enableLBT));
    LOG("RSSI: %s\n", Lora::rssi_enable_str(cfg.transMode.enableRSSI));
    LOG("Repeater mode: %s\n", Lora::repeater_enable_str(cfg.transMode.enableRepeater));
    LOG("Fixed mode: %s\n", Lora::fixed_transmiss_str(cfg.transMode.fixedTransmission));
}