#include "SettingsReader.h"
#include "../Flash/Flash.h"
#include <stdexcept>

SettingsReader::SettingsReader(const std::string& filename)
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
        json_ = nlohmann::json::object();
    }
}

SettingsReader::~SettingsReader() = default;

int64_t SettingsReader::get_i(std::string_view param)
{
    return section_->at(param).get<int64_t>();
}

int64_t SettingsReader::get_i(std::string_view param, int64_t defValue)
{
    if (section_->contains(param)) {
        return section_->at(param).get<int64_t>();
    }
    return defValue;
}

float SettingsReader::get_f(std::string_view param)
{
    return section_->at(param).get<float>();
}

float SettingsReader::get_f(std::string_view param, float defValue)
{
    if (section_->contains(param)) {
        return section_->at(param).get<float>();
    }
    return defValue;
}

std::string SettingsReader::get_s(std::string_view param)
{
    return section_->at(param).get<std::string>();
}

std::string SettingsReader::get_s(std::string_view param, const std::string& defValue)
{
    if (section_->contains(param)) {
        return section_->at(param).get<std::string>();
    }
    return defValue;
}

bool SettingsReader::get_b(std::string_view param)
{
    return section_->at(param).get<bool>();
}

bool SettingsReader::get_b(std::string_view param, bool defValue)
{
    if (section_->contains(param)) {
        return section_->at(param).get<bool>();
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
        nlohmann::json::parse(jsonStr);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

void SettingsReader::setSection(std::string_view section)
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
