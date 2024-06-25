#pragma once

#include "DisplaySettings.h"
#include <stdint.h>
#include <string>

class U8G2_ST7920_128X64_F_SW_SPI;

class Display
{
public:
    Display(); 
    ~Display();
    void init(const DisplaySettings& settings);
    void clear();
    void drawStr(uint8_t x, uint8_t y, const std::string& str);
    void flush();
    uint8_t getDisplayWidth() const;
    uint8_t getDisplayHeight() const;
    uint8_t getStrWidth(const char* str) const;
    uint8_t getMaxCharWidth() const;
    uint8_t getMaxCharHeight() const;
    
private:
    U8G2_ST7920_128X64_F_SW_SPI* u8g2_;
};