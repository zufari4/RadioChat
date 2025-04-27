#pragma once

#include "UISettings.h"
#include "UIContext.h"
#include "../Keyboard/KeyCommand.h"
#include "UIPageType.h"
#include <stdint.h>
#include <string>
#include <memory>

class UIPageBase
{
public:
    UIPageBase(UIPageType type, UIPageType parent, const UIContext* context);
    virtual ~UIPageBase();
    virtual void draw();
    virtual void onChar(uint16_t symbol);
    virtual void onKeyCommand(KeyCommand cmd);
    UIPageType getType() const;

protected:
    UIPageType type_;
    UIPageType parent_;
    const UIContext* ctx_;
};