#include "Display.h"
#include "Configuration.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

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
    u8g2_ = new U8G2_ST7920_128X64_F_HW_SPI(U8G2_R0, settings.pins.RS);
    u8g2_->begin();

    u8g2_->setFont(DISPLAY_FONT);
    u8g2_->setFontRefHeightExtendedText();
    u8g2_->setDrawColor(1);
    u8g2_->setFontPosTop();
    u8g2_->setFontDirection(0);
}

void Display::clear()
{
    u8g2_->clearBuffer();
}

void Display::drawStr(uint8_t x, uint8_t y, const std::string& str)
{
    u8g2_->drawUTF8(x, y, str.c_str());
}

void Display::flush()
{
    u8g2_->sendBuffer();
}
