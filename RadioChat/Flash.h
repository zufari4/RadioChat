#pragma once

#include "FlashSettings.h"

class Flash
{
public:
    Flash();
    ~Flash();
    void init(const FlashSettings& settings);

private:
    FlashSettings settings_;
};