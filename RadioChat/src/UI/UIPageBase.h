#pragma once

#include "UIPageType.h"
#include "UISettings.h"
#include "../Keyboard/KeyCommand.h"
#include <stdint.h>
#include <string>


class Display;


class UIPageBase
{
public:
    UIPageBase(UIPageType type, const UISettings& settings, Display* display);
    virtual ~UIPageBase();
    virtual void draw();
    virtual void onChar(uint16_t symbol);
    virtual void onKeyCommand(KeyCommand cmd);

protected:
    void drawText(uint8_t x, uint8_t y, const std::string& str);
    uint8_t getMaxStrLen() const;
    uint8_t getTextHeight() const;
    uint8_t getMaxCountLines() const;
    uint8_t getTextWidth(const std::string& str) const;

private:
    const UIPageType type_;
    UISettings settings_;
    Display* display_;
    const uint8_t maxStrLen_;
    const uint8_t  textHeight_;
    const uint8_t maxCountLines_;
};