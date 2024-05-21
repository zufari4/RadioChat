#pragma once

#include <stdint.h>
#include <vector>

namespace Melody
{
    enum class Name: uint8_t
    {
        Packman,
        Nokia,
    };

    struct Step
    {
        uint16_t frequency;
        int16_t  duration;
    };

    struct Info
    {
        Name name;
        std::vector<Step> steps;
    };

    const Info& getMelody(Name melodyName);
}
