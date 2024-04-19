#include "WiFi.h"
#include "Logger.h"
#include <WiFi.h>

WiFiModule::WiFiModule()
{
}

WiFiModule::~WiFiModule()
{
}

void WiFiModule::init(const WiFiSettings& settings)
{
    LOG("-- Initialize Wi-Fi --\n");
    if (!settings.enable) {
        //WiFiMode(WIFI_STA);
        WiFi.disconnect(); 
        WiFi.mode(WIFI_OFF);
    }
    LOG("Wi-Fi module is %s\n", settings.enable ? "enabled" : "disabled");
}