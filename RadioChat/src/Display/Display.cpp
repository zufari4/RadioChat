#include "Display.h"
#include "../Logger/Logger.h"
#include "../Configuration.h"
#include "../Settings/Settings.h"
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

void Display::init(Settings& settingsMgr)
{
    LOG_INF("-- Initialize display --"); 
    auto settings = loadSettings(settingsMgr);

    u8g2_ = new U8G2_ST7920_128X64_F_SW_SPI(U8G2_R0, settings.pins.E, settings.pins.R_W, settings.pins.RS, settings.pins.RST);
    u8g2_->begin();
    u8g2_->setFont(DISPLAY_FONT);
    u8g2_->setFontRefHeightExtendedText();
    u8g2_->setDrawColor(1);
    u8g2_->setFontPosTop();
    u8g2_->setFontDirection(0);

    pinMode(settings.pins.BLA, OUTPUT);
    analogWrite(settings.pins.BLA, settings.brightnessLevel);
}

void Display::clear()
{
    u8g2_->clearBuffer();
}

void Display::drawStr(uint8_t x, uint8_t y, std::string_view str)
{
    if (!str.empty()) {
        u8g2_->drawUTF8(x, y, str.data());
    }
}

void Display::drawButtonWith(uint8_t x, uint8_t y, uint8_t width, std::string_view str)
{
    u8g2_->drawButtonUTF8(x, y, U8G2_BTN_INV, width, 0,  0, str.data());
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

uint8_t Display::getStrWidth(std::string_view str) const
{
    return u8g2_->getUTF8Width(str.data());
}

uint8_t Display::getMaxCharWidth() const
{
    return u8g2_->getMaxCharWidth();
}

uint8_t Display::getMaxCharHeight() const
{
    return u8g2_->getMaxCharHeight();
}

DisplaySettings Display::loadSettings(Settings& settings)
{
    auto props = settings.display();
    DisplaySettings settingsData;
    settingsData.pins.RS = Settings::get_i(eDisplayPinRS, props);
    settingsData.pins.R_W = Settings::get_i(eDisplayPinRW, props);
    settingsData.pins.E = Settings::get_i(eDisplayPinE, props);
    settingsData.pins.RST = Settings::get_i(eDisplayPinRST, props);
    settingsData.pins.BLA = Settings::get_i(eDisplayPinBLA, props);
    settingsData.brightnessLevel = Settings::get_i(eDisplayBrightnessLevel, props);
    return settingsData;
}
