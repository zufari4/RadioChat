#include "Flash.h"
#include "../Logger/Logger.h"
#include "../Configuration.h"
#include <FS.h>
#include <SD.h>
#include <SPI.h>

Flash::Flash()
    : state_(State::NotInit)
{
}

Flash::~Flash()
{
}

void Flash::init()
{
    LOG_INF("--- Init flash ---");

    if (!SD.begin(FLASH_PIN_CS)) {
        LOG_ERR("Card Mount Failed");
        state_ = State::NotInit;
        return;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        LOG_ERR("No SD card attached");
        state_ = State::NoCard;
        return;
    }

    state_ = State::Init;
    LOG_INF("Flash is initialized");
}

void Flash::printInfo() const
{
    LOG_INF("Print flash info");
    if (state_ == State::NotInit) {
        LOG_ERR("Flash is not init");
        return;
    }
    else if (state_ == State::NoCard) {
        LOG_ERR("No SD card attached");
        return;
    }

    uint8_t cardType = SD.cardType();
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);

    std::string cardTypeStr;
    if(cardType == CARD_MMC){
        cardTypeStr = "MMC";
    } else if(cardType == CARD_SD){
        cardTypeStr = "SDSC";
    } else if(cardType == CARD_SDHC){
        cardTypeStr = "SDHC";
    } else {
        cardTypeStr = "UNKNOWN";
    }

    LOG_INF("SD Card Type: %s", cardTypeStr.c_str());
    LOG_INF("SD Card Size: %llu MB", cardSize);
}

std::string Flash::read(std::string_view filename)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);
    LOG_DBG("Read file %s", filename.data());

    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return "";
    }

    if (!SD.exists(filename.data())) {
        LOG_ERR("File not found (%s)", filename.data());
        return "";
    }

    File file = SD.open(filename.data(), FILE_READ);
    if (!file) {
        LOG_ERR("Failed to open file for reading (%s)", filename.data());
        return "";
    }

    std::string res;
    while (file.available()) {
        res += (char)file.read();
    }
    file.close();

    return res;
}

bool Flash::write(std::string_view filename, const std::string& content)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);

    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return false;
    }

    File file = SD.open(filename.data(), FILE_WRITE);
    if (!file) {
        return false;
    }
    if (file.print(content.c_str()) == 0) {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool Flash::append(const std::string& filename, const std::string& content)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);

    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return false;
    }

    File file = SD.open(filename.c_str(), FILE_APPEND, true);
    if (!file) {
        return false;
    }
    if (file.print(content.c_str()) == 0) {
        file.close();
        return false;
    }
    file.close();

    return true;
}

void Flash::listDir(const char* dirname)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);

    LOG_INF("Listing directory: %s\n", dirname);

    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return;
    }

    File root = SD.open(dirname);
    if (!root){
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()){
        Serial.println("Not a directory");
        root.close();
        return;
    }

    File file = root.openNextFile();
    while (file) 
    {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            std::string path = file.path();
            file.close();
            listDir(path.c_str());
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            file.close();
        }
        
        file = root.openNextFile();
    }
    if (file) {
        file.close();
    }
    if (root) {
        root.close();
    }
}

Flash& Flash::instance()
{
    static Flash instance;
    return instance;
}

bool Flash::exists(const std::string& filename)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);
    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return false;
    }
    return SD.exists(filename.c_str());
}

void Flash::mkdir(const std::string& dirname)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);
    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return;
    }
    if (!SD.exists(dirname.c_str())) {
        if (!SD.mkdir(dirname.c_str())) {
            LOG_ERR("Failed to create directory (%s)", dirname.c_str());
        }
        else {
            LOG_DBG("Directory created (%s)", dirname.c_str());
        }
    }
    else {
        LOG_DBG("Directory already exists (%s)", dirname.c_str());
    }
}

void Flash::remove(const std::string& filename)
{
    std::lock_guard<std::recursive_mutex> lock(fsMutex_);
    LOG_DBG("Remove file %s", filename.c_str());

    if (state_ != State::Init) {
        LOG_ERR("Flash is not init");
        return;
    }

    if (SD.exists(filename.c_str())) {
        if (!SD.remove(filename.c_str())) {
            LOG_ERR("Failed to remove file (%s)", filename.c_str());
        } else {
            LOG_DBG("File removed (%s)", filename.c_str());
        }
    } else {
        LOG_DBG("File not found (%s)", filename.c_str());
    }
}
