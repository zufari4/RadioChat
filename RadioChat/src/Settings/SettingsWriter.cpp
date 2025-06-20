#include "SettingsWriter.h"
#include "../Flash/Flash.h"
#include "../Logger/Logger.h"
#include <stdexcept>

SettingsWriter::SettingsWriter(std::string_view filename)
    : filename_(filename)
{
    try
    {
        std::string jsonStr = FLASH.read(filename);
        if (jsonStr.empty()) {
            json_ = nlohmann::json::object();
        }
        else {
            json_ = nlohmann::json::parse(jsonStr);
        }
    }
    catch (const std::exception& e) {
        LOG_ERR("Failed to parse JSON file %s: %s", filename.data(), e.what());
        json_ = nlohmann::json::object();
    }
}

SettingsWriter::~SettingsWriter() = default;

void SettingsWriter::set_i(std::string_view param, int64_t value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_f(std::string_view param, float value)
{
    (*section_)[param] = value;
}
void SettingsWriter::set_s(std::string_view param, std::string_view value)
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
    if (FLASH.write(filename_, jsonStr)) {
        return true;
    }
    else {
        LOG_ERR("Failed to save settings to file '%s'", filename_.c_str());
    }
    return false;
}

void SettingsWriter::setSection(std::string_view section)
{
    if (currentSectionName_ == section) {
        return;
    }
    if (json_.find(section) == json_.end()) {
        json_[section] = nlohmann::json::object();
        section_ = &json_[section];
    }
    else {
        section_ = &json_.at(section);
    }
    currentSectionName_ = section;
}
