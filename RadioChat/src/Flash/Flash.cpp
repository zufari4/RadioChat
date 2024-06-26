#include "Flash.h"
#include "../Logger/Logger.h"
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

void Flash::init(const FlashSettings& settings)
{
    LOG_INF("--- Init flash ---");
    settings_ = settings;

    if (!SD.begin(settings_.pins.cs)) {
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
    LOG_INF("Success");
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

std::string Flash::read(const std::string& filename)
{
    LOG_INF("Read file %s", filename.c_str());

    File file = SD.open(filename.c_str());
    if (!file) {
        LOG_ERR("Failed to open file for reading (%s)", filename.c_str());
        return "";
    }

    std::string res;
    while (file.available()) {
        res += (char)file.read();
    }
    file.close();

    return res;
}

bool Flash::create(const std::string& filename, const std::string& content)
{
    LOG_INF("Write to file %s", filename.c_str());

    File file = SD.open(filename.c_str(), FILE_WRITE);
    if (!file) {
        LOG_ERR("Failed to open file for writing (%s)", filename.c_str());
        return false;
    }
    if (file.print(content.c_str()) == 0) {
        LOG_ERR("Write failed (%s)", content.c_str());
        return false;
    }
    file.close();

    return true;
}

void Flash::listDir(const char* dirname)
{
    LOG_INF("Listing directory: %s\n", dirname);

    File root = SD.open(dirname);
    if (!root){
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) 
    {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            listDir(file.path());
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}