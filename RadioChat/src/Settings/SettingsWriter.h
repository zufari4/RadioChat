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
    SettingsWriter(std::string_view filename);
    ~SettingsWriter();
    void setSection(std::string_view section);
    void set_i(std::string_view param, int64_t value);
    void set_f(std::string_view param, float value);
    void set_s(std::string_view param, std::string_view value);
    void set_b(std::string_view param, bool value);
    bool save();

private:
    std::string filename_;
    nlohmann::json json_;
    nlohmann::json* section_ = nullptr;
    std::string currentSectionName_;
};