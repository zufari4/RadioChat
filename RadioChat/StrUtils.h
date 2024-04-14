#pragma once

#include <stdint.h>
#include <string>

namespace utils
{
std::string to_str(uint16_t utf8symbol);
void add_to_str(uint16_t utf8symbol, std::string& out);
}
