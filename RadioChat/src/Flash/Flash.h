#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <mutex>

#define FLASH Flash::instance()

class Flash
{
public:  
    enum class State
    {
        NotInit,
        NoCard,
        Init,
    };

    Flash();
    ~Flash();
    static Flash& instance();
    void init();
    void printInfo() const;
    std::string read(std::string_view filename);
    bool write(std::string_view filename, const std::string& content);
    bool append(const std::string& filename, const std::string& content);
    bool exists(const std::string& filename);
    void mkdir(const std::string& dirname);
    void remove(const std::string& filename);
    std::vector<std::string> readLastNLines(const std::string& filename, uint32_t start, uint32_t n);

private:
    State state_;
    std::recursive_mutex fsMutex_;

    void listDir(const char* dirname);
};