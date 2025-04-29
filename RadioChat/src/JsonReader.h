#pragma once

#include <string_view>
#include <string>
#include <cstdint>
#include <memory>

namespace nlohmann
{
    class json;
}

class JsonReader
{
public:
    JsonReader(const std::string& filename);
    virtual ~JsonReader();
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
    double get_d(std::string_view param);
    double get_d(std::string_view param, double defValue);
    std::string get_s(std::string_view param);
    std::string get_s(std::string_view param, const std::string& defValue);
    bool get_b(std::string_view param);
    bool get_b(std::string_view param, bool defValue);

protected:
    std::unique_ptr<nlohmann::json> json_;
};