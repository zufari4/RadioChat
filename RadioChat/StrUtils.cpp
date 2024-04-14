#include "StrUtils.h"

namespace utils
{

std::string to_str(uint16_t utf8symbol)
{
    uint8_t tmp[3];
    if (utf8symbol <= 0x00FF) {
        tmp[0] = (uint8_t)utf8symbol;
        tmp[1] = 0;
    }
    else {
        uint8_t* ptr = (uint8_t*)(&utf8symbol);
        tmp[0] = ptr[1];
        tmp[1] = ptr[0];
        tmp[2] = 0;
    }
    return (char*)(&tmp);
}

void add_to_str(uint16_t utf8symbol, std::string& out)
{
    out += to_str(utf8symbol);
}

}