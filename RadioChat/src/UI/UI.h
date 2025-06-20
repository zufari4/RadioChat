#pragma once

#include "UIContext.h"
#include "../Keyboard/KeyCommand.h"
#include "../Settings/Property.h"
#include <memory>
#include <mutex>

class Settings;
class UIPageBase;

class UI
{
public:
    UI();
    ~UI();
    void init(const UIContext& context);
    void draw();
    void onChar(uint16_t symbol);
    void onKeyCommand(KeyCommand cmd);
    void onIncomingMessage(const std::string& message, uint16_t senderAddress, uint16_t destAddress);
    void showTypingMessage(uint16_t destinationAddress);
    void showPropertyList(const PropertyMap& properties);
    void showEditProperty(const Property& prop);
    void showChatContact(uint16_t address);
    void setCurrentPage(UIPageType pageType);

private:
    std::unique_ptr<UIPageBase> createPage(UIPageType pageType);
    void setCurrentPage(std::unique_ptr<UIPageBase> pageType);
    UISettings loadSettings(Settings& settings) const;

    std::mutex pageMutex_;
    UIContext ctx_;
    std::unique_ptr<UIPageBase> currentPage_;
};