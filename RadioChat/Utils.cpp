#include "Utils.h"
#include <time.h>

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

void pop_back_utf8(std::string& utf8)
{
    if(utf8.empty())
        return;

    const char* start = utf8.c_str();
    const char* cp = start + utf8.size();

    while (--cp >= start && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {}
    if (cp >= start) utf8.resize(cp - start);
}

const char* next(const char* utf8)
{
    const char* s = utf8 + 1;
    while (*s) {
        if ((*s & 0xc0) != 0x80) {
            break;
        }
        else s++;
    }
    return s;
}

int utf8_len(const std::string& utf8)
{
    int len = 0;
    if (utf8.empty()) return len;
    
    const char* s = utf8.c_str();
    while (*s) len += (*s++ & 0xc0) != 0x80;
    return len;
}

const char* currentDateTime()
{
    static char buffer[25];
    static time_t now;
    static tm tstruct;

    now = time(nullptr);
    tstruct = *localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tstruct);
    return buffer;
}

}