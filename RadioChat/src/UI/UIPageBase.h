#pragma once

#include "UIPageType.h"
#include "UISettings.h"
#include "UIContext.h"
#include "../Keyboard/KeyCommand.h"
#include <stdint.h>
#include <string>


class Display;


class UIPageBase
{
public:
    UIPageBase(UIPageType type, const UIContext* context);
    virtual ~UIPageBase();
    virtual void draw();
    virtual void onChar(uint16_t symbol);
    virtual void onKeyCommand(KeyCommand cmd);

protected:
    const UIContext* ctx_;

private:
    const UIPageType type_;
};