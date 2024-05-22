#include "Esp.h"
#include "Logger.h"
#include <WiFi.h>

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
        WiFi.begin(settings_.wifi.ssid, settings_.wifi.pass);
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