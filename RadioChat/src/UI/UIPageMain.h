#pragma once

#include "BaseMenu.h"

class UIPageMain: public BaseMenu
{
public:
    UIPageMain(const UIContext* contex);
    ~UIPageMain();
    void draw() override;

private:
    void chatClick();
    void settingsClick();
    void rebootClick();
    void logsClick();
    void notesClick();

    float prevVBatt_ = 0;
    std::string prevVBattStr_;
    char currentVBatt_[16];
};