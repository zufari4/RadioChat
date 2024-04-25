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
    LOG("Init ESP\n");
    settings_ = settings;

    if (!settings_.wifi.enable) {
        WiFi.disconnect(); 
        WiFi.mode(WIFI_OFF);
    }
    els {
        WiFi.begin(settings_.wifi.ssid, settings_.wifi.pass);
    }
    LOG("Wi-Fi module is %s\n", settings_.wifi.enable ? "enabled" : "disabled");

    if (!settings_.bluethoose.enable) {
        btStop();
    }
    else {
        btStart();
    }
    LOG("Bluethoose module is %s\n", settings_.wifi.enable ? "enabled" : "disabled");
}