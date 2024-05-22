#pragma once

#include "DisplaySettings.h"
#include "Configuration.h"
#include <stdint.h>
#include <string>

class DISPLAY_MODEL;

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
    DISPLAY_MODEL* u8g2_;
};