#pragma once

#include <cstdint>

enum PropertyType : uint8_t
{
    eNone,
    // Logging
    eLogTraceLevel,
    eLogToSerial,
    eLogToFile,
    eLogPath,
    eLogMaxCountLogs,
    eLogMaxCountLines,
    eLogMaxMessageSize,
    // Esp
    eEspWiFiEnable,
    eEspWiFiSSID,
    eEspWiFiPassword,
    eEspBluetoothEnable,
    // Keyboard
    eKeyboardPinShLd,
    eKeyboardPinQh,
    eKeyboardPinClk,
    eKeyboardLang,
    eKeyboardMaxKeyNum,
    eKeyboardFnKey,
    eKeyboardEnterKey,
    eKeyboardCountRegisters,
    // Display
    eDisplayPinRS,
    eDisplayPinRW,
    eDisplayPinE,
    eDisplayPinRST,
    eDisplayPinBLA,
    eDisplayBrightnessLevel,
    // Battery
    eBatteryPinVoltage,
    eBatteryMaxVoltage,
    eBatteryMaxADC,
    eBatteryCFactor,
    eBatteryCheckInterval,
    // Contacts
    eContactsPath,
    eContactsFilename,
    // Flash
    eFlashPinMISO,
    eFlashPinMOSI,
    eFlashPinSCK,
    eFlashPinCS,
    // Led
    eLedPinOn,
    eLedInterval,
    // Radio
    eRadioPinAUX,
    eRadioPinM0,
    eRadioPinM1,
    eRadioPinRX,
    eRadioPinTX,
    eRadioChannel,
    eRadioSelfAddress,
    eRadioUARTBaudrate,
    eRadioUARTTimeoutMs,
    eRadioUARTParity,
    eRadioSubPacketSize,
    eRadioAirRate,
    // Sound
    eSoundPinIO,
    eSoundTempo,
    eSoundEnable,
    eSoundEnablePlayOnPowerOn,
    // UI
    eUITextHeight,
    eUITextCarriageChar,
    eUITextCarriageShowTime,
};
