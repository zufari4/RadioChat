#pragma once

#include <stdint.h>
#include <vector>
#include <string_view>

namespace Melody
{
    enum class Name: uint8_t
    {
        Undefined = 0,
        Packman,
        Nokia,
        Accept,
        PackmanShort,
    };

    struct Step
    {
        uint16_t frequency;
        int16_t  duration;
    };

    struct Info
    {
        Name name;
        std::string_view nameStr;
        std::vector<Step> steps;
    };

    Info getMelody(Name melodyName);
}
