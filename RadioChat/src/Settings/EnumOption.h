#pragma once

#include <string>
#include <vector>

struct EnumOptionItem
{
    std::string caption;
    std::string value;
};

using EnumOption = std::vector<EnumOptionItem>;