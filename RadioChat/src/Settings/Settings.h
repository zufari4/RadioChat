#pragma once

#include "Property.h"

class SettingsWriter;

class Settings
{
public:
    Settings();
    ~Settings();
    void init(std::string_view filename);
    PropertyMap logger();
    PropertyMap esp();
    PropertyMap keyboard();
    PropertyMap display();
    PropertyMap radio();
    PropertyMap sound();
    PropertyMap led();
    PropertyMap battery();
    PropertyMap contacts();
    PropertyMap ui();

    static int64_t get_i(PropertyType type, const PropertyMap& props);
    static float get_f(PropertyType type, const PropertyMap& props);
    static std::string get_s(PropertyType type, const PropertyMap& props);
    static bool get_b(PropertyType type, const PropertyMap& props);

    void setProperty(const Property& prop);
private:
    void setProp(SettingsWriter& opt, const Property& option);
    PropertyMap read(const std::vector<Property>& defProps);

    std::vector<Property> getLogDefaultProperties();
    std::vector<Property> getEspDefaultProperties();
    std::vector<Property> getKeyboardDefaultProperties();
    std::vector<Property> getDisplayDefaultProperties();
    std::vector<Property> getRadioDefaultProperties();
    std::vector<Property> getSoundDefaultProperties();
    std::vector<Property> getLedDefaultProperties();
    std::vector<Property> getBatteryDefaultProperties();
    std::vector<Property> getContactsDefaultProperties();
    std::vector<Property> getUiDefaultProperties();

    static EnumOption getLogTraceLevelOptions();
    static EnumOption getLangOptions();
    static EnumOption getRadioAirRateOptions();
    static EnumOption getRadioUARTParityOptions();
    static EnumOption getRadioUARTBaudrateOptions();
    static EnumOption getRadioSubPacketSizeOptions();


    std::string filename_;
};