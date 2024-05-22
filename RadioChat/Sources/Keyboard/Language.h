#pragma once

#include <stdint.h>

enum class Language: uint8_t
{
    Russian = 0,
    English = 1,
    Count
};

const char* lang2str(Language lang);