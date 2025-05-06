#pragma once

#include <string_view>
#include <string>
#include <cstdint>
#include <memory>
#include <map>
#include <vector>
#include <json.hpp>

class SettingsWriter
{
public:
    SettingsWriter(const std::string& filename, std::string_view section);
    ~SettingsWriter();
    void set_i(std::string_view param, int32_t value);
    void set_u(std::string_view param, uint32_t value);
    void set_i64(std::string_view param, int64_t value);
    void set_u64(std::string_view param, uint64_t value);
    void set_f(std::string_view param, float value);
    void set_s(std::string_view param, const std::string& value);
    void set_b(std::string_view param, bool value);
    bool save();

private:
    std::string filename_;
    nlohmann::json json_;
    nlohmann::json* section_ = nullptr;
};