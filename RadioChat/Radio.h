#include <stdint.h>
#pragma once

#include "RadioSettings.h"
#include "Lora.h"

class Radio
{
public:
    Radio();
    ~Radio();
    void init(const RadioSettings& settings);
    bool isInit() const;

private:
    bool setMode(Lora::Mode mode);
    bool waitReady();
    bool isReady();
    bool getConfiguration(Lora::Configuration& out);
    bool writeProgramCommand(Lora::PROGRAM_COMMAND cmd, Lora::REGISTER_ADDRESS addr, Lora::PACKET_LENGHT pl);
    bool readData(void* out, uint8_t dataSize);
    void traceConfig(const Lora::Configuration& cfg) const;

    RadioSettings settings_;
    Lora::Mode currentMode_;
    bool isInit_;
};