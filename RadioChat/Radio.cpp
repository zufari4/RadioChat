#include "Radio.h"
#include "Logger.h"
#include "RadioCommand.h"
#include <HardwareSerial.h>
#include <Arduino.h>
#include <stdint.h>

Radio::Radio()
    : currentMode_(Lora::Mode::Undefined)
    , isInit_(false)
    , newMessageID_(0)
{
}

Radio::~Radio()
{

}

void Radio::init(const RadioSettings& settings, OnNewMessageCallback onNewMessage, OnMessageDeliveryCallback onMessageDelivered, OnPingDone onPingDone)
{
    LOG("-- Initialize radio module --\n");

    settings_ = settings;
    onNewMessage_ = onNewMessage;
    onMessageDelivered_ = onMessageDelivered;
    onPingDone_ = onPingDone;

    LOG("RX %u TX %u AUX %u M0 %u M1 %u\n", settings_.pins.RX, settings_.pins.TX, settings_.pins.AUX, settings_.pins.M0, settings_.pins.M1);
    LOG("channel %u address %u\n", settings_.channel, settings_.address);
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
    if (settings_.channel != cfg.chan) {
        if (!setChannel(cfg.chan)) {
            return;
        }
    }
    if (settings_.address != Lora::get_address(cfg)) {
        if (!setAddress(settings_.address)) {
            return;
        }
    }
    isInit_ = setMode(Lora::Mode::Transfer);
}

bool Radio::isInit() const
{
    return isInit_;
}

bool Radio::setChannel(uint8_t channel)
{
    LOG("Set channel %u\n", channel);
    auto setter = [channel](Lora::Configuration& cfg) 
    {
        cfg.chan = channel;
    };
    if (!setConfiguration(setter)) {
        LOG("Can't set channel\n");
        return false;
    } 
    return true;
}

bool Radio::setAddress(uint16_t addr)
{
    LOG("Set address %u\n", addr);
    auto setter = [addr](Lora::Configuration& cfg) 
    {
        cfg.addh = (addr>>8) & 0xff;
        cfg.addl = addr & 0xff;
    };
    if (!setConfiguration(setter)) {
        LOG("Can't set address\n");
        return false;
    } 
    return true;
}

bool Radio::setConfiguration(std::function<void(Lora::Configuration& cfg)> setter)
{
    Lora::Mode prevMode = currentMode_;
    bool res = false;

    do {
        if (!setMode(Lora::Mode::Configuration)) {
            break;
        }
        Lora::Configuration cfg;
        if (!getConfiguration(cfg)) {
            break;
        }

        cfg.command = Lora::PROGRAM_COMMAND::WRITE_CFG_PWR_DWN_SAVE;
        cfg.address = Lora::REGISTER_ADDRESS::REG_ADDRESS_CFG;
        cfg.length  = Lora::PACKET_LENGHT::PL_CONFIGURATION;

        setter(cfg);

        if (!writeData(&cfg, sizeof(Lora::Configuration))) {
            break;
        }
        if (!readData(&cfg, sizeof(Lora::Configuration))) {
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
        res = true;
    } while (false);

    res = setMode(prevMode) && res;
    return res;
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
        res = true;
	} while(false);

    return setMode(prevMode) && res;
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

void Radio::check()
{
    if (!Serial2.available()) {
        return;
    }

    uint8_t sender_addh;
    uint8_t sender_addl;
    RadioCommand cmd;

    if (!readData(&sender_addh, 1, false)) {
        LOG("Can't read sender_addh\n");
        return;
    }
    if (!readData(&sender_addl, 1, false)) {
        LOG("Can't read sender_addl\n");
        return;
    }
    if (!readData(&cmd, 1, false)) {
        LOG("Can't read cmd\n");
        return;
    }
    uint16_t sender = Lora::get_address(sender_addh, sender_addl);

    switch (cmd)
    {
    case RadioCommand::MessageNew: {
        LOG("Radio: receive new message\n");
        std::string text;
        uint8_t msgID = receiveText(text);
        if (msgID > 0) {
            sendDelivered(sender, msgID);
            onNewMessage_(text, msgID);
        }
        break;
    }
    case RadioCommand::MessageDelivered: {
        LOG("Radio: receive message delivered\n");
        uint8_t msgID;
        if (readData(&msgID, 1)) {
            LOG("Radio: delivered %u\n", msgID);
            onMessageDelivered_(sender, msgID);
        }   
        break;
    }
    case RadioCommand::Ping: {
        LOG("Radio: receive ping from %u\n", sender);
        sendPingDelivered(sender);
        break;
    }
    case RadioCommand::PingDelivered: {
        uint32_t delay = millis() - startPing_[sender];
        LOG("Radio: ping delivered from %u delay %u\n", sender, delay);
        onPingDone_(sender, delay);
        break;
    }
    default: {
        LOG("Radio: Unknown command %u", (uint8_t)cmd);
        break;
    }
    }
}

bool Radio::setMode(Lora::Mode mode)
{
    if (mode == Lora::Mode::Undefined) {
        return true;
    }
    if (currentMode_ == mode) {
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

    currentMode_ = mode;
    return res;
}

bool Radio::writeProgramCommand(Lora::PROGRAM_COMMAND cmd, Lora::REGISTER_ADDRESS addr, Lora::PACKET_LENGHT pl)
{
    LOG("Write command\n"); 
	uint8_t data[3] = {(uint8_t)cmd, (uint8_t)addr, (uint8_t)pl};
    if (!writeData(data, 3)) {
        LOG("Can't write command\n");
        return false;
    }
    return true;
}

bool Radio::writeData(void* data, uint8_t dataSize)
{
    uint8_t* ptr = (uint8_t*)data;
    if (dataSize > 0) {
        LOGX("--> ");
        for (uint8_t i = 0; i < dataSize; ++i) {
            LOGX("%02X ", ptr[i]);
        }
        LOGX("\n");
    }
	uint8_t size = Serial2.write(ptr, dataSize);
    //delay(50);  //need ti check
    if (size != dataSize) {
        LOG("Wrong size of write data: %u\n", size);
        return false;
    }
	else if (size == dataSize) {
        if (!waitReady()) {
            return false;
        }
        //while (Serial2.available()) {
        //    Serial2.read();
        //}
        return true;
    }
}

bool Radio::readData(void* out, uint8_t dataSize, bool needWaitReady /*= true*/) 
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

	bool res = needWaitReady ? waitReady() : true;
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

void Radio::fillHeader(std::vector<uint8_t>& out, uint16_t destAddr, RadioCommand command)
{
    out.push_back(Lora::get_addr_h(destAddr));
    out.push_back(Lora::get_addr_l(destAddr));
    out.push_back(settings_.channel);

    out.push_back(Lora::get_addr_h(settings_.address));
    out.push_back(Lora::get_addr_l(settings_.address));
    out.push_back((uint8_t)command);
}

bool Radio::ping(uint16_t addr, uint32_t& delay)
{
    if (!sendPing(addr)) {
        return false;
    }
    startPing_[addr] = millis();
}

uint8_t Radio::sendText(const std::string& text, uint16_t destAddr /*= BROADCAST_ADDRESS*/)
{
    LOG("Send text '%s'\n", text.c_str());
    // byte dest ADDH
    // byte dest ADDL
    // byte CHAN

    // byte sender ADDH
    // byte sender ADDL
    // byte cmd

    // byte msg_id
    // byte msg_len
    // byte message[msg_len];
    std::vector<uint8_t> data;
    uint8_t msgID = ++newMessageID_;

    fillHeader(data, destAddr, RadioCommand::MessageNew);
    data.push_back(msgID);
    data.push_back((uint8_t)text.size());
    data.insert(data.end(), text.begin(), text.end());

    bool res = writeData(data.data(), data.size());
    return res ? msgID : 0;
}

uint8_t Radio::receiveText(std::string& text)
{
    uint8_t msg_id;
    uint8_t msg_len;
    if (!readData(&msg_id, 1, false)) {
        LOG("Can't read msg_id\n");
        return 0;
    }
    if (!readData(&msg_len, 1, false)) {
        LOG("Can't read msg_len %u\n", msg_id);
        return 0;
    }
    std::vector<uint8_t> data(msg_len + 1, '\0');        
    if (!readData(data.data(), msg_len, true)) {
        LOG("Can't read msg %u\n", msg_id);
        return 0;
    }
    text.assign(data.begin(), data.end());
    LOG("Receive '%s' %u\n", text.c_str(), msg_id);
    return msg_id;
}

void Radio::sendDelivered(uint16_t sender, uint8_t msgID)
{
    LOG("Send delivered to %u msgID %u\n", sender, msgID);
    // byte dest ADDH
    // byte dest ADDL
    // byte CHAN

    // byte sender ADDH
    // byte sender ADDL
    // byte cmd

    // byte msg_id

    std::vector<uint8_t> data;
    fillHeader(data, sender, RadioCommand::MessageDelivered);
    data.push_back(msgID);

    if (!writeData(data.data(), data.size())) {
        LOG("Can't send delivered status\n");
    }
}

bool Radio::sendPing(uint16_t dest)
{
    LOG("Send ping to %u\n", dest);
    // byte dest ADDH
    // byte dest ADDL
    // byte CHAN

    // byte sender ADDH
    // byte sender ADDL
    // byte cmd

    std::vector<uint8_t> data;
    fillHeader(data, dest, RadioCommand::Ping);

    if (!writeData(data.data(), data.size())) {
        LOG("Can't send ping\n");
        return false;
    }

    return true;
}

void Radio::sendPingDelivered(uint16_t sender)
{
    LOG("Send ping delivered to %u\n", sender);
    // byte dest ADDH
    // byte dest ADDL
    // byte CHAN

    // byte sender ADDH
    // byte sender ADDL
    // byte cmd

    std::vector<uint8_t> data;
    fillHeader(data, sender, RadioCommand::PingDelivered);

    if (!writeData(data.data(), data.size())) {
        LOG("Can't send ping delivered\n");
    }
}