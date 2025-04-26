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
    void onKeyCommand(KeyCommand cmd) override;

private:
    void drawMainMenu();
    void chatClick(Item* item);
    void contactsClick(Item* item);
    void settingsClick(Item* item);
    void rebootClick(Item* item);
    void logsClick(Item* item);
    void notesClick(Item* item);

    float prevVBatt_ = 0;
    std::string prevVBattStr_;
    char currentVBatt_[16];
    std::unique_ptr<UIPageBase> subPage_;
    std::recursive_mutex pageMtx_;
};