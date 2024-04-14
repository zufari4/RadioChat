#pragma once

#include "DisplaySettings.h"
#include <stdint.h>
#include <string>

class U8G2_ST7920_128X64_F_HW_SPI;

class Display
{
public:
    Display(); 
    ~Display();
    void init(const DisplaySettings& settings);
    void clear();
    void drawStr(uint8_t x, uint8_t y, const std::string& str);
    void flush();

private:
    U8G2_ST7920_128X64_F_HW_SPI* u8g2_;
};