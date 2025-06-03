#pragma once

#include "DisplaySettings.h"
#include <stdint.h>
#include <string>
#include <string_view>

class U8G2_ST7920_128X64_F_SW_SPI;
class Settings;

class Display
{
public:
    Display(); 
    ~Display();
    void init(Settings& settings);
    void clear();
    void drawStr(uint8_t x, uint8_t y, std::string_view str);
    void drawButtonWith(uint8_t x, uint8_t y, uint8_t width, std::string_view str);
    void flush();
    uint8_t getDisplayWidth() const;
    uint8_t getDisplayHeight() const;
    uint8_t getStrWidth(std::string_view str) const;
    uint8_t getMaxCharWidth() const;
    uint8_t getMaxCharHeight() const;
    
private:
    DisplaySettings loadSettings(Settings& settings);

    U8G2_ST7920_128X64_F_SW_SPI* u8g2_;
};