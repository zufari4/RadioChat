#include "Esp.h"
#include "../Logger/Logger.h"
#include <WiFi.h>
#include <Arduino.h>

Esp::Esp()
{
}

Esp::~Esp()
{
}

void Esp::init(const EspSettings& settings)
{
    LOG_INF("Init ESP");
    settings_ = settings;

    if (!settings_.wifi.enable) {
        WiFi.disconnect(); 
        WiFi.mode(WIFI_OFF);
    }
    else {
        WiFi.begin(settings_.wifi.ssid.c_str(), settings_.wifi.pass.c_str());
    }
    LOG_INF("Wi-Fi module is %s", settings_.wifi.enable ? "enabled" : "disabled");

    if (!settings_.bluethoose.enable) {
        btStop();
    }
    else {
        btStart();
    }
    LOG_INF("Bluethoose module is %s", settings_.bluethoose.enable ? "enabled" : "disabled");
}