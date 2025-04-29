#include "JsonReader.h"
#include <json.hpp>
#include <stdexcept>
#include <fstream>

JsonReader::JsonReader(const std::string& filename)
{
    std::ifstream f(filename);
    if (f.is_open()) {
        //json_ = nlohmann::json::parse(f);
    }
    else {
        //json_ = nlohmann::json::object();
    }
}

JsonReader::~JsonReader()
{
}

int32_t JsonReader::get_i(std::string_view param)
{
    return json_->at(param).get<int32_t>();
}

int32_t JsonReader::get_i(std::string_view param, int32_t defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<int32_t>();
    }
    return defValue;
}

uint32_t JsonReader::get_u(std::string_view param)
{
    return json_->at(param).get<uint32_t>();
}

uint32_t JsonReader::get_u(std::string_view param, uint32_t defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<uint32_t>();
    }
    return defValue;
}

int64_t JsonReader::get_i64(std::string_view param)
{
    return json_->at(param).get<int64_t>();
}

int64_t JsonReader::get_i64(std::string_view param, int64_t defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<int64_t>();
    }
    return defValue;
}

uint64_t JsonReader::get_u64(std::string_view param)
{
    return json_->at(param).get<uint64_t>();
}

uint64_t JsonReader::get_u64(std::string_view param, uint64_t defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<uint64_t>();
    }
    return defValue;
}

float JsonReader::get_f(std::string_view param)
{
    return json_->at(param).get<float>();
}

float JsonReader::get_f(std::string_view param, float defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<float>();
    }
    return defValue;
}

double JsonReader::get_d(std::string_view param)
{
    return json_->at(param).get<double>();
}

double JsonReader::get_d(std::string_view param, double defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<double>();
    }
    return defValue;
}

std::string JsonReader::get_s(std::string_view param)
{
    return json_->at(param).get<std::string>();
}

std::string JsonReader::get_s(std::string_view param, const std::string& defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<std::string>();
    }
    return defValue;
}

bool JsonReader::get_b(std::string_view param)
{
    return json_->at(param).get<bool>();
}

bool JsonReader::get_b(std::string_view param, bool defValue)
{
    if (json_->contains(param)) {
        return json_->at(param).get<bool>();
    }
    return defValue;
}
