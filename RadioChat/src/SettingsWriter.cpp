#include "SettingsWriter.h"
#include "Utils.h"
#include "Logger/Logger.h"
#include <stdexcept>

SettingsWriter::SettingsWriter(const std::string& filename, std::string_view section)
{
    try
    {
        std::string jsonStr = utils::readFile(filename);
        if (jsonStr.empty()) {
            json_ = nlohmann::json::object();
            json_[section] = nlohmann::json::object();
            section_ = &json_[section];
        }
        else {
            json_ = nlohmann::json::parse(jsonStr);
            if (json_.find(section) == json_.end()) {
                json_[section] = nlohmann::json::object();
                section_ = &json_[section];
            }
            else {
                section_ = &json_.at(section);
            }
        }
    }
    catch (const std::exception& e) {
        LOG_ERR("Failed to parse JSON file %s: %s", filename.c_str(), e.what());
        json_ = nlohmann::json::object();
        json_[section] = nlohmann::json::object();
        section_ = &json_[section];
    }
}

SettingsWriter::~SettingsWriter() = default;

void SettingsWriter::set_i(std::string_view param, int32_t value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_u(std::string_view param, uint32_t value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_i64(std::string_view param, int64_t value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_u64(std::string_view param, uint64_t value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_f(std::string_view param, float value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_s(std::string_view param, const std::string& value)
{
    (*section_)[param] = value;
}

void SettingsWriter::set_b(std::string_view param, bool value)
{
    (*section_)[param] = value;
}

bool SettingsWriter::save()
{
    std::string jsonStr = json_.dump(4);
    LOG_DBG("save settings json '%s'", jsonStr.c_str());
    if (utils::writeToFile(filename_, jsonStr)) {
        return true;
    }
    return false;
}