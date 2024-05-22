#pragma once

#include "EspSettings.h"

class Esp
{
public:
    Esp();
    ~Esp();
    void init(const EspSettings& settings);

private:
    EspSettings settings_;
};