#include "SettingsReader.h"
#include "Flash/Flash.h"
#include <stdexcept>

SettingsReader::SettingsReader(const std::string& filename, std::string_view section)
{
    try
    {
        std::string jsonStr = FLASH.read(filename);
        if (jsonStr.empty()) {
            json_ = nlohmann::json::object();
        }
        else {
            json_ = nlohmann::json::parse(jsonStr);
            if (json_.find(section) == json_.end()) {
                json_ = nlohmann::json::object();
            }
            else {
                json_ = json_.at(section);
            }
        }
    }
    catch (const std::exception& e) {
        json_ = nlohmann::json::object();
    }
}

SettingsReader::~SettingsReader() = default;

int32_t SettingsReader::get_i(std::string_view param)
{
    return json_.at(param).get<int32_t>();
}

int32_t SettingsReader::get_i(std::string_view param, int32_t defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<int32_t>();
    }
    return defValue;
}

uint32_t SettingsReader::get_u(std::string_view param)
{
    return json_.at(param).get<uint32_t>();
}

uint32_t SettingsReader::get_u(std::string_view param, uint32_t defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<uint32_t>();
    }
    return defValue;
}

int64_t SettingsReader::get_i64(std::string_view param)
{
    return json_.at(param).get<int64_t>();
}

int64_t SettingsReader::get_i64(std::string_view param, int64_t defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<int64_t>();
    }
    return defValue;
}

uint64_t SettingsReader::get_u64(std::string_view param)
{
    return json_.at(param).get<uint64_t>();
}

uint64_t SettingsReader::get_u64(std::string_view param, uint64_t defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<uint64_t>();
    }
    return defValue;
}

float SettingsReader::get_f(std::string_view param)
{
    return json_.at(param).get<float>();
}

float SettingsReader::get_f(std::string_view param, float defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<float>();
    }
    return defValue;
}

std::string SettingsReader::get_s(std::string_view param)
{
    return json_.at(param).get<std::string>();
}

std::string SettingsReader::get_s(std::string_view param, const std::string& defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<std::string>();
    }
    return defValue;
}

bool SettingsReader::get_b(std::string_view param)
{
    return json_.at(param).get<bool>();
}

bool SettingsReader::get_b(std::string_view param, bool defValue)
{
    if (json_.contains(param)) {
        return json_.at(param).get<bool>();
    }
    return defValue;
}

bool SettingsReader::isValid(const std::string& filename)
{
    try
    {
        std::string jsonStr = FLASH.read(filename);
        if (jsonStr.empty()) {
            return false;
        }
        auto json = nlohmann::json::parse(jsonStr);
        (json);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
