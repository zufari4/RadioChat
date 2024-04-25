#include "Display.h"
#include "Logger.h"
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

    u8g2_ = new DISPLAY_MODEL(U8G2_R0, settings.pins.RS);
    u8g2_->begin();

    u8g2_->setFont(DISPLAY_FONT);
    u8g2_->setFontRefHeightExtendedText();
    u8g2_->setDrawColor(1);
    u8g2_->setFontPosTop();
    u8g2_->setFontDirection(0);

    LOG_INF("RS pin %u\n", settings.pins.RS);
    LOG_INF("Display width %u height %u\n", getDisplayWidth(), getDisplayHeight());
    LOG_INF("Max char width %u height %u\n", getMaxCharWidth(), getMaxCharHeight());
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