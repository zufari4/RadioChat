#pragma once

#include <stdint.h>
#include <HardwareSerial.h>
#include "../Configuration.h"

struct RadioSettings // Lora E22-400T33D
{
    struct Pins
    {
        uint8_t AUX = 32;
        uint8_t M0  = 27;
        uint8_t M1  = 26;
        uint8_t RX  = 25;
        uint8_t TX  = 33;
    } pins;
    uint8_t  channel = 23;
    uint16_t selfAddress = DEVICE_ADDRESS;
    struct UART
    {
        uint32_t baudrate = 9600;
        uint16_t timeoutMs= 1000;
        uint32_t parity = SERIAL_8N1;
    } uart;
    uint8_t subPacketSize = 0; // Lora::SUB_PACKET_SETTING::e240;
};