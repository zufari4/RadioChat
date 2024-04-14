#pragma once

#include "UISettings.h"

class Display;

class UI
{
public:
    UI();
    ~UI();
    void init(const UISettings& settings, Display* display);
    void draw();

private:
    Display* display_;
};