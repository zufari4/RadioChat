#pragma once

struct EspSettings
{
    struct WiFi {
        bool enable = false;
        char ssid[11] = "radio_chat";
        char pass[11] = "radio_chat";
    } wifi;
    struct Bluethoose {
        bool enable = false;
    } bluethoose;
};
