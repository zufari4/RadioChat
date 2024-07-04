#pragma once

#include "BaseMenu.h"
#include <vector>
#include <memory>
#include <mutex>

class UIPageChat: public BaseMenu
{
public:
    UIPageChat(const UIContext* context);
    ~UIPageChat();
    void onKeyCommand(KeyCommand cmd) override;
    void draw() override;

private:
    void onSharedChatClick(Item& item);
    void onNewContactClick(Item& item);
    void onContactClick(Item& item);

    std::unique_ptr<UIPageBase> currentPage_;
    std::recursive_mutex pageMtx_;
};