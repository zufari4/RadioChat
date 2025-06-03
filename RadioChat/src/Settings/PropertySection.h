#pragma once

#include <string_view>

enum class PropertySection
{
    None,
    Log,
    Esp,
    Keyboard,
    Display,
    Battery,
    Contacts,
    Flash,
    Led,
    Radio,
    Sound,
    UI,
};

std::string_view propSectionToStr(PropertySection section);