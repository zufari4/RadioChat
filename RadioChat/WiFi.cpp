#include "WiFi.h"
#include "Logger.h"
#include <ESP8266WiFi.h>

WiFiModule::WiFiModule()
{
}

WiFiModule::~WiFiModule()
{
}

void WiFiModule::init(const WiFiSettings& settings)
{
    if (!settings.enable) {
        LOG("Disable WiFi module\n");
        WiFiMode(WIFI_STA);
        WiFi.disconnect(); 
        WiFi.mode(WIFI_OFF);
    }
}