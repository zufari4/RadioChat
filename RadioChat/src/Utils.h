#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace utils
{
std::string to_str(uint16_t utf8symbol);
void add_to_str(uint16_t utf8symbol, std::string& out);
uint8_t pop_back_utf8(std::string& utf8);
const char* next(const char* utf8);
bool is_ascii(const char* symbol);
int utf8_len(const std::string& utf8);
uint8_t datetime_str(char* out, uint8_t outSize);
std::string datetime_str();
const char* to_str(bool val);
std::string bits2str(uint8_t val);
// Function to split a UTF-8 string into a vector of strings with a maximum length
std::vector<std::string> splitUtf8String(const std::string& input, size_t maxLength);
}
