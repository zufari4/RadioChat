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
    SettingsReader(const std::string& filename, std::string_view section);
    ~SettingsReader();

    int32_t get_i(std::string_view param);
    int32_t get_i(std::string_view param, int32_t defValue);
    uint32_t get_u(std::string_view param);
    uint32_t get_u(std::string_view param, uint32_t defValue);
    int64_t get_i64(std::string_view param);
    int64_t get_i64(std::string_view param, int64_t defValue);
    uint64_t get_u64(std::string_view param);
    uint64_t get_u64(std::string_view param, uint64_t defValue);
    float get_f(std::string_view param);
    float get_f(std::string_view param, float defValue);
    std::string get_s(std::string_view param);
    std::string get_s(std::string_view param, const std::string& defValue);
    bool get_b(std::string_view param);
    bool get_b(std::string_view param, bool defValue);

    static bool isValid(const std::string& filename);
protected:
   nlohmann::json json_;
};