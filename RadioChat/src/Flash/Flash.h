#pragma once

#include "FlashSettings.h"
#include <vector>
#include <string>

namespace fs
{
class File;
}

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
    void init(const FlashSettings& settings);
    void printInfo() const;
    bool exist(const std::string& path);
    bool createDir(const std::string& path);
    std::string read(const std::string& filename);
    bool create(const std::string& filename, const std::string& content);
    bool write(fs::File& file, const std::string& text);
    fs::File open(const std::string& filename);

private:
    State state_;
    FlashSettings settings_;
    
    void listDir(const char* dirname);
};