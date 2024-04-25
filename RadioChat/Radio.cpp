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
    LOG_INF("-- Initialize radio module --");

    settings_ = settings;
    onNewMessage_ = onNewMessage;
    onMessageDelivered_ = onMessageDelivered;
    onPingDone_ = onPingDone;

    LOG_DBG("RX %u TX %u AUX %u M0 %u M1 %u", settings_.pins.RX, settings_.pins.TX, settings_.pins.AUX, settings_.pins.M0, settings_.pins.M1);
    LOG_INF("channel %u address %u", settings_.channel, settings_.address);
    LOG_DBG("baudrate %u timeout %u parity %u", settings_.uart.baudrate, settings_.uart.timeoutMs, settings_.uart.parity);
    
    LOG_INF("Setup pins");
    pinMode(settings_.pins.AUX, INPUT);
	pinMode(settings_.pins.M0, OUTPUT);
    pinMode(settings_.pins.M1, OUTPUT);

    LOG_INF("Wait device is ready");
    if (!waitReady()) {
        return;
    }
    
    LOG_INF("Init UART");
    Serial2.setTimeout(settings_.uart.timeoutMs);
    Serial2.begin(settings_.uart.baudrate, settings_.uart.parity, settings_.pins.TX, settings_.pins.RX);
    while (!Serial2) { } //wait for serial port to connect
    
    Lora::Configuration cfg;
    if (!getConfiguration(cfg)) {
        LOG_ERR("Can't get radio configuration");
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
    LOG_INF("Set channel %u", channel);
    auto setter = [channel](Lora::Configuration& cfg) 
    {
        cfg.chan = channel;
    };
    if (!setConfiguration(setter)) {
        LOG_ERR("Can't set channel");
        return false;
    } 
    return true;
}

bool Radio::setAddress(uint16_t addr)
{
    LOG_INF("Set address %u", addr);
    auto setter = [addr](Lora::Configuration& cfg) 
    {
        cfg.addh = (addr>>8) & 0xff;
        cfg.addl = addr & 0xff;
    };
    if (!setConfiguration(setter)) {
        LOG_ERR("Can't set address");
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
		    LOG_ERR("Wrong format");
            break;
        }
        if (Lora::PROGRAM_COMMAND::RETURNED_COMMAND != cfg.command || 
            Lora::REGISTER_ADDRESS::REG_ADDRESS_CFG != cfg.address || 
            Lora::PACKET_LENGHT::PL_CONFIGURATION   != cfg.length) {
            LOG_ERR("Head is not recognized");
		    break;
	    }
        res = true;
    } while (false);

    res = setMode(prevMode) && res;
    return res;
}

bool Radio::getConfiguration(Lora::Configuration& out)
{
    LOG_INF("Get configuration");
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
		    LOG_ERR("Wrong format");
            break;
        }
        if (Lora::PROGRAM_COMMAND::RETURNED_COMMAND != cfg.command || 
            Lora::REGISTER_ADDRESS::REG_ADDRESS_CFG != cfg.address || 
            Lora::PACKET_LENGHT::PL_CONFIGURATION   != cfg.length) {
            LOG_ERR("Head is not recognized");
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
        LOG_ERR("Timeout of wait AUX ready");
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
        LOG_ERR("Can't read sender_addh");
        return;
    }
    if (!readData(&sender_addl, 1, false)) {
        LOG_ERR("Can't read sender_addl");
        return;
    }
    if (!readData(&cmd, 1, false)) {
        LOG_ERR("Can't read cmd");
        return;
    }
    uint16_t sender = Lora::get_address(sender_addh, sender_addl);

    switch (cmd)
    {
    case RadioCommand::MessageNew: {
        LOG_INF("Radio: receive new message");
        std::string text;
        uint8_t msgID = receiveText(text);
        if (msgID > 0) {
            sendDelivered(sender, msgID);
            onNewMessage_(text, msgID);
        }
        break;
    }
    case RadioCommand::MessageDelivered: {
        LOG_INF("Radio: receive message delivered");
        uint8_t msgID;
        if (readData(&msgID, 1)) {
            LOG_INF("Radio: delivered %u", msgID);
            onMessageDelivered_(sender, msgID);
        }   
        break;
    }
    case RadioCommand::Ping: {
        LOG_INF("Radio: receive ping from %u", sender);
        sendPingDelivered(sender);
        break;
    }
    case RadioCommand::PingDelivered: {
        uint32_t delay = millis() - startPing_[sender];
        LOG_INF("Radio: ping delivered from %u delay %u", sender, delay);
        onPingDone_(sender, delay);
        break;
    }
    default: {
        LOG_ERR("Radio: Unknown command %u", (uint8_t)cmd);
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
    LOG_INF("Set radio mode: %s", radio_mode_str(mode));

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
        LOG_ERR("Can't wait status of set mode");
    }

    currentMode_ = mode;
    return res;
}

bool Radio::writeProgramCommand(Lora::PROGRAM_COMMAND cmd, Lora::REGISTER_ADDRESS addr, Lora::PACKET_LENGHT pl)
{
    LOG_INF("Write command"); 
	uint8_t data[3] = {(uint8_t)cmd, (uint8_t)addr, (uint8_t)pl};
    if (!writeData(data, 3)) {
        LOG_ERR("Can't write command");
        return false;
    }
    return true;
}

bool Radio::writeData(void* data, uint8_t dataSize)
{
    uint8_t* ptr = (uint8_t*)data;
    traceTraffic("-->", ptr, dataSize);
	uint8_t size = Serial2.write(ptr, dataSize);
    //delay(50);  //need ti check
    if (size != dataSize) {
        LOG_ERR("Wrong size of write data: %u", size);
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
        traceTraffic("<--", dst, len);
    }
	if (len != dataSize) {
		LOG_ERR("Wrong length of receive. Need %u actual %u", dataSize, len);
        return false;
	}

	bool res = needWaitReady ? waitReady() : true;
    if (!res) {
        LOG_ERR("Can't wait status of read bytes");
    }
    return res;
}

void Radio::traceConfig(const Lora::Configuration& cfg) const
{
	LOG_DBG("HEAD: %02X %02X %02X", (uint8_t)cfg.command, (uint8_t)cfg.address, (uint8_t)cfg.length);
	LOG_DBG("AddH : %02X", cfg.addh);  
	LOG_DBG("AddL : %02X", cfg.addl);  
	LOG_DBG("NetID: %02X", cfg.netid); 
	LOG_DBG("Channel: %s", Lora::channel_str(cfg.chan).c_str());
	LOG_DBG("UART parity: %s", Lora::parity_str(cfg.speed.uartParity));
	LOG_DBG("UART baud rate: %s", Lora::bps_type_str(cfg.speed.uartBaudRate));
    LOG_DBG("Air data rate: %s", Lora::air_rate_str(cfg.speed.airDataRate));
    LOG_DBG("Subpacket size: %s", Lora::sub_pack_str(cfg.option.subPacketSetting));
    LOG_DBG("Transmission power: %s", Lora::transmission_power_str(cfg.option.transmissionPower));
    LOG_DBG("RSSI ambient noise: %s", Lora::rssi_noize_str(cfg.option.RSSIAmbientNoise));
    LOG_DBG("WOR period: %s", Lora::wor_period_str(cfg.transMode.WORPeriod));
    LOG_DBG("WOR control: %s", Lora::wor_control_str(cfg.transMode.WORTransceiverControl));
    LOG_DBG("LBT: %s", Lora::lbt_enable_str(cfg.transMode.enableLBT));
    LOG_DBG("RSSI: %s", Lora::rssi_enable_str(cfg.transMode.enableRSSI));
    LOG_DBG("Repeater mode: %s", Lora::repeater_enable_str(cfg.transMode.enableRepeater));
    LOG_DBG("Fixed mode: %s", Lora::fixed_transmiss_str(cfg.transMode.fixedTransmission));
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
    LOG_INF("Send text '%s'", text.c_str());
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
        LOG_ERR("Can't read msg_id");
        return 0;
    }
    if (!readData(&msg_len, 1, false)) {
        LOG_ERR("Can't read msg_len %u", msg_id);
        return 0;
    }
    std::vector<uint8_t> data(msg_len + 1, '\0');        
    if (!readData(data.data(), msg_len, true)) {
        LOG_ERR("Can't read msg %u", msg_id);
        return 0;
    }
    text.assign(data.begin(), data.end());
    LOG_INF("Receive '%s' %u", text.c_str(), msg_id);
    return msg_id;
}

void Radio::sendDelivered(uint16_t sender, uint8_t msgID)
{
    LOG_INF("Send delivered to %u msgID %u", sender, msgID);
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
        LOG_ERR("Can't send delivered status");
    }
}

bool Radio::sendPing(uint16_t dest)
{
    LOG_INF("Send ping to %u", dest);
    // byte dest ADDH
    // byte dest ADDL
    // byte CHAN

    // byte sender ADDH
    // byte sender ADDL
    // byte cmd

    std::vector<uint8_t> data;
    fillHeader(data, dest, RadioCommand::Ping);

    if (!writeData(data.data(), data.size())) {
        LOG_ERR("Can't send ping");
        return false;
    }

    return true;
}

void Radio::sendPingDelivered(uint16_t sender)
{
    LOG_INF("Send ping delivered to %u", sender);
    // byte dest ADDH
    // byte dest ADDL
    // byte CHAN

    // byte sender ADDH
    // byte sender ADDL
    // byte cmd

    std::vector<uint8_t> data;
    fillHeader(data, sender, RadioCommand::PingDelivered);

    if (!writeData(data.data(), data.size())) {
        LOG_ERR("Can't send ping delivered");
    }
}

void Radio::traceTraffic(const char* direction, uint8_t* data, uint8_t dataSize) const
{
    if (dataSize > 0 && Logger::instance().getLogLevel() == LogTraceLevel::Debug) {
        std::string str;
        char hex[4];
        for (uint8_t i = 0; i < dataSize; ++i) {
            sprintf(hex, "%02X ", data[i]);
            str += hex;
        }
        LOG_DBG("%s %s", direction, str.c_str());
    }
}