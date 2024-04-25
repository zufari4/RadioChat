#pragma once

#include "RadioSettings.h"
#include "RadioCommand.h"
#include "Lora.h"
#include <functional>
#include <stdint.h>
#include <map>

class Radio
{
public:
    using OnNewMessageCallback = std::function<void(const std::string& text, uint8_t msgID)>;
    using OnMessageDeliveryCallback = std::function<void(uint16_t address, uint8_t msgID)>;
    using OnPingDone = std::function<void(uint16_t address, uint32_t delay)>;

    Radio();
    ~Radio();
    void init(const RadioSettings& settings, OnNewMessageCallback onNewMessage, OnMessageDeliveryCallback onMessageDelivered, OnPingDone onPingDone);
    void check();
    bool isInit() const;
    bool setChannel(uint8_t channel);
    bool setAddress(uint16_t addr);
    uint8_t sendText(const std::string& text, uint16_t address = BROADCAST_ADDRESS);
    bool ping(uint16_t addr, uint32_t& delay);

private:
    bool setMode(Lora::Mode mode);
    bool waitReady();
    bool isReady();
    bool getConfiguration(Lora::Configuration& out);
    bool writeProgramCommand(Lora::PROGRAM_COMMAND cmd, Lora::REGISTER_ADDRESS addr, Lora::PACKET_LENGHT pl);
    bool readData(void* out, uint8_t dataSize, bool needWaitReady = true);
    bool writeData(void* data, uint8_t dataSize);
    void traceConfig(const Lora::Configuration& cfg) const;
    bool setConfiguration(std::function<void(Lora::Configuration& cfg)> fn);
    uint8_t receiveText(std::string& text);
    void sendDelivered(uint16_t sender, uint8_t msgID);
    bool sendPing(uint16_t dest);
    void sendPingDelivered(uint16_t sender);
    void fillHeader(std::vector<uint8_t>& out, uint16_t destAddr, RadioCommand command);
    void traceTraffic(const char* direction, uint8_t* data, uint8_t dataSize) const;

    RadioSettings settings_;
    Lora::Mode currentMode_;
    bool isInit_;
    uint8_t newMessageID_;
    OnNewMessageCallback onNewMessage_;
    OnMessageDeliveryCallback onMessageDelivered_;
    OnPingDone onPingDone_;
    std::map<uint16_t, uint32_t> startPing_;
};