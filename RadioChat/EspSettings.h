#pragma once

struct EspSettings
{
    struct WiFi {
        bool enable = false;
        char ssid[9] = { 'e','s','p','_','c','h','a','t', '\0'};
        char pass[9] = { 'e','s','p','_','c','h','a','t', '\0'};
    } wifi;
    struct Bluethoose {
        bool enable = false;
    } bluethoose;
};
