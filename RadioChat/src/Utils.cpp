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

bool is_ascii(const char* symbol)
{
    return (*symbol & 0xc0) != 0x80;
}

const char* next(const char* utf8)
{
    const char* s = utf8 + 1;
    while (*s) {
        if (is_ascii(s)) {
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
    std::string res;

    for (uint8_t b = 0; b < 8; ++b) {
        bool isBitSet = val & (1 << b);
        res += isBitSet ? '1' : '0';
    }
    
    return res;
}

// Function to split a UTF-8 string into a vector of strings with a maximum length
std::vector<std::string> splitUtf8String(const std::string& input, size_t maxLength)
{
    std::vector<std::string> result;
    std::string currentChunk;
    size_t currentChunkSize = 0;

    for (size_t i = 0; i < input.size(); ) {
        unsigned char c = static_cast<unsigned char>(input[i]);
        size_t charSize = 0;

        // Determine the size of the UTF-8 character
        if ((c & 0x80) == 0) {
            charSize = 1; // 1-byte character (ASCII)
        }
        else if ((c & 0xE0) == 0xC0) {
            charSize = 2; // 2-byte character
        }
        else if ((c & 0xF0) == 0xE0) {
            charSize = 3; // 3-byte character
        }
        else if ((c & 0xF8) == 0xF0) {
            charSize = 4; // 4-byte character
        }
        else { // Invalid UTF-8 encoding detected
            break;
        }

        // If adding this character exceeds the current chunk size, start a new chunk
        if (currentChunkSize >= maxLength) {
            result.push_back(currentChunk);
            currentChunk.clear();
            currentChunkSize = 0;
        }

        // Append the character to the current chunk
        currentChunk.append(input.substr(i, charSize));
        currentChunkSize += 1;
        i += charSize;
    }

    // Add the last chunk if not empty
    if (!currentChunk.empty()) {
        result.push_back(currentChunk);
    }

    return result;
}



}