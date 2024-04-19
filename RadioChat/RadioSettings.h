#pragma once

#include <stdint.h>
#include <HardwareSerial.h>

struct RadioSettings // Lora E22-400T33D
{
    struct Pins
    {
        uint8_t AUX = 18;
        uint8_t M0  = 21;
        uint8_t M1  = 19;
        uint8_t RX  = 22;
        uint8_t TX  = 23;
    } pins;
    uint8_t channel = 23;
    uint8_t speed   = 1;
    uint8_t address = 0;
    struct UART
    {
        uint32_t baudrate = 9600;
        uint16_t timeoutMs= 100;
        uint32_t parity = SERIAL_8N1;
    } uart;
};