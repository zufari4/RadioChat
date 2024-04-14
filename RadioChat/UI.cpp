#include "UI.h"
#include "Display.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::init(const UISettings& settings, Display* display)
{
    display_ = display;
}

void UI::draw()
{
    display_->clear();
    //display_->drawStr(0, 0, str);
    display_->flush();
}