#pragma once

#include "BaseMenu.h"
#include <memory>
#include <mutex>

class UIPageMain: public BaseMenu
{
public:
    UIPageMain(const UIContext* contex);
    ~UIPageMain();
    void draw() override;

protected:
    void onItemClick(uint8_t itemIndex) override;

private:
    void updateBatteryVoltage();

    float prevVBatt_ = 0;
    std::string prevVBattStr_;
    char currentVBatt_[16] = {0};
};