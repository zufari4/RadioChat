#pragma once

#include "RadioSettings.h"
#include "RadioDef.h"

class Radio
{
public:
    Radio();
    ~Radio();
    bool init(const RadioSettings& settings);

private:
    bool setMode(RadioDef::RadioMode mode);
    bool waitReady();
    bool isReady();
    bool getConfiguration(RadioDef::Configuration& out);
    bool writeProgramCommand(RadioDef::PROGRAM_COMMAND cmd, RadioDef::REGISTER_ADDRESS addr, RadioDef::PACKET_LENGHT pl);
    bool receiveData(void *out, uint8_t dataSize);
    void traceConfig(const RadioDef::Configuration& cfg) const;

    RadioSettings settings_;
    RadioDef::RadioMode currentMode_;
};