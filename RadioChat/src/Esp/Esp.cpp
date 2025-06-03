#include "Esp.h"
#include "../Logger/Logger.h"
#include "../Settings/Settings.h"
#include <WiFi.h>
#include <Arduino.h>

Esp::Esp()
{
}

Esp::~Esp()
{
}

void Esp::init(Settings& settings)
{
    LOG_INF("Init ESP");
    loadSettings(settings);

    if (!settings_.wifi.enable) {
        WiFi.disconnect(); 
        WiFi.mode(WIFI_OFF);
    }
    else {
        WiFi.begin(settings_.wifi.ssid.c_str(), settings_.wifi.pass.c_str());
    }
    if (!settings_.bluethoose.enable) {
        btStop();
    }
    else {
        btStart();
    }
}

void Esp::loadSettings(Settings& settings)
{
    auto props = settings.esp();
    settings_.wifi.enable = Settings::get_b(eEspWiFiEnable, props);
    settings_.wifi.ssid = Settings::get_s(eEspWiFiSSID, props);
    settings_.wifi.pass = Settings::get_s(eEspWiFiPassword, props);
    settings_.bluethoose.enable = Settings::get_b(eEspBluetoothEnable, props);
}
