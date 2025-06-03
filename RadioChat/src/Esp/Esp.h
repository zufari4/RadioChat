#pragma once

#include "EspSettings.h"

class Settings;

class Esp
{
public:
    Esp();
    ~Esp();
    void init(Settings& settings);

private:
    void loadSettings(Settings& settings);
    EspSettings settings_;
};