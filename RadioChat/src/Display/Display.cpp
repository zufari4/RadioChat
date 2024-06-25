#include "Display.h"
#include "../Logger/Logger.h"
#include "../Configuration.h"
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>


Display::Display()
    : u8g2_(nullptr)
{
}

Display::~Display()
{
    if (u8g2_ != nullptr) {
        delete u8g2_;
        u8g2_ = nullptr;
    }
}

void Display::init(const DisplaySettings& settings)
{
    LOG_INF("-- Initialize display --"); 
    u8g2_ = new U8G2_ST7920_128X64_F_SW_SPI(U8G2_R0, settings.pins.E, settings.pins.R_W, settings.pins.RS, settings.pins.RST);
    u8g2_->begin();
    u8g2_->setFont(DISPLAY_FONT);
    u8g2_->setFontRefHeightExtendedText();
    u8g2_->setDrawColor(1);
    u8g2_->setFontPosTop();
    u8g2_->setFontDirection(0);

    pinMode(settings.pins.BLA, OUTPUT);
    analogWrite(settings.pins.BLA, settings.brightnessLevel);

    LOG_INF("Pins: RS %u R/W %u E %u RST %u BLA %u", settings.pins.RS, settings.pins.R_W, settings.pins.E, settings.pins.RST, settings.pins.BLA);
    LOG_DBG("Display width %u height %u", getDisplayWidth(), getDisplayHeight());
    LOG_DBG("Max char width %u height %u", getMaxCharWidth(), getMaxCharHeight());
    LOG_INF("Brightness level: %u", settings.brightnessLevel);
}

void Display::clear()
{
    u8g2_->clearBuffer();
}

void Display::drawStr(uint8_t x, uint8_t y, const std::string& str)
{
    if (!str.empty()) {
        u8g2_->drawUTF8(x, y, str.c_str());
    }
}

void Display::flush()
{
    u8g2_->sendBuffer();
}

uint8_t Display::getDisplayWidth() const
{
    return u8g2_->getDisplayWidth();
}

uint8_t Display::getDisplayHeight() const
{
    return u8g2_->getDisplayHeight();
}

uint8_t Display::getStrWidth(const char* str) const
{
    return u8g2_->getStrWidth(str);
}

uint8_t Display::getMaxCharWidth() const
{
    return u8g2_->getMaxCharWidth();
}

uint8_t Display::getMaxCharHeight() const
{
    return u8g2_->getMaxCharHeight();
}