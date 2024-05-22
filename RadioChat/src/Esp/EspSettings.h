#pragma once

#include <string>

struct EspSettings
{
    struct WiFi {
        bool enable = false;
        std::string ssid = "radio_chat";
        std::string pass = "radio_chat";
    } wifi;
    struct Bluethoose {
        bool enable = false;
    } bluethoose;
};
