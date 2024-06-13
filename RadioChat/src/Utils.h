#pragma once

#include <stdint.h>
#include <string>

namespace utils
{
std::string to_str(uint16_t utf8symbol);
void add_to_str(uint16_t utf8symbol, std::string& out);
void pop_back_utf8(std::string& utf8);
const char* next(const char* utf8);
int utf8_len(const std::string& utf8);
uint8_t datetime_str(char* out, uint8_t outSize);
std::string datetime_str();
const char* to_str(bool val);
bool fileIsExists(const std::string& filename);
std::string readFile(const std::string& filename);
bool writeToFile(const std::string& filename, const std::string& content);
}
