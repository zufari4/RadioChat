#pragma once

#include <string_view>
#include <string>
#include <cstdint>
#include <memory>
#include <map>
#include <vector>
#include <json.hpp>

class SettingsReader
{
public:
    SettingsReader(const std::string& filename);
    ~SettingsReader();

    void setSection(std::string_view section);
    int64_t get_i(std::string_view param);
    int64_t get_i(std::string_view param, int64_t defValue);
    float get_f(std::string_view param);
    float get_f(std::string_view param, float defValue);
    std::string get_s(std::string_view param);
    std::string get_s(std::string_view param, const std::string& defValue);
    bool get_b(std::string_view param);
    bool get_b(std::string_view param, bool defValue);

    static bool isValid(const std::string& filename);
protected:
   nlohmann::json json_;
   nlohmann::json* section_ = nullptr;
   std::string currentSectionName_;
};