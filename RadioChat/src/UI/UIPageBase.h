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
    enum class ExitStatus
    {
        Undefined,
        Cancel,
        Accept
    };

    UIPageBase(UIPageType type, const UIContext* context);
    virtual ~UIPageBase();
    virtual void draw();
    virtual void onChar(uint16_t symbol);
    virtual void onKeyCommand(KeyCommand cmd);
    ExitStatus getExitStatus() const;

protected:
    const UIContext* ctx_;
    void setExitStatus(ExitStatus status);

private:
    const UIPageType type_;
    ExitStatus exitStatus_ = ExitStatus::Undefined;
};