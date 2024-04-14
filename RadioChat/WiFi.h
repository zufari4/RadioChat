#pragma once

#include "WiFiSettings.h"

class WiFiModule
{
public:
    WiFiModule();
    ~WiFiModule();
    void init(const WiFiSettings& settings);
};