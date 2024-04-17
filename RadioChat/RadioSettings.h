#pragma once

#include <stdint.h>

struct RadioSettings // Lora E22-400T33D
{
    struct Pins 
    {
        uint8_t AUX = 4;
        uint8_t M0  = 16;
        uint8_t M1  = 5;
    } pins;
    uint8_t channel = 23;
    uint8_t speed   = 1;
    uint8_t address = 0;
    uint16_t uartBaudrate = 9600;
    uint16_t uartTimeoutMs= 100;
};