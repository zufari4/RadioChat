#include <string>
#include "Utils.h"
#include "Logger/Logger.h"
#include <SD.h>
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

uint8_t datetime_str(char* out, uint8_t outSize)
{
    time_t now;
    tm tstruct;

    now = time(nullptr);
    tstruct = *localtime(&now);
    return strftime(out, outSize, "%Y-%m-%d %X", &tstruct);
}

std::string datetime_str()
{
    char buff[32];
    datetime_str(buff, sizeof(buff));
    return buff;
}

const char* to_str(bool val)
{
    return val ? "true" : "false";
}

bool fileIsExists(const std::string &filename)
{
    return SD.exists(filename.c_str());
}

std::string readFile(const std::string &filename)
{
    LOG_INF("Read file %s", filename.c_str());

    File file = SD.open(filename.c_str());
    if (!file) {
        LOG_ERR("Failed to open file for reading (%s)", filename.c_str());
        return "";
    }

    std::string res;
    while (file.available()) {
        res += (char)file.read();
    }
    file.close();

    return res;
}

bool writeToFile(const std::string &filename, const std::string &content)
{
    LOG_INF("Write to file %s", filename.c_str());

    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (!file) {
        LOG_ERR("Failed to open file for writing (%s)", filename.c_str());
        return false;
    }
    if (file.print(content.c_str()) == 0) {
        LOG_ERR("Write failed (%s)", content.c_str());
        return false;
    }
    file.close();

    return true;
}

std::string bits2str(uint8_t val)
{
    uint8_t i, j;
    std::string res;

    for (uint8_t b = 0; b < 8; ++b) {
        bool isBitSet = val & (1 << b);
        res += isBitSet ? '1' : '0';
    }
    
    return res;
}

}