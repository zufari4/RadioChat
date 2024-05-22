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

    if (!SD.begin()) {
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

    LOG_INF("SD Card Type: ");
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_MMC){
        LOG_INF("MMC");
    } else if(cardType == CARD_SD){
        LOG_INF("SDSC");
    } else if(cardType == CARD_SDHC){
        LOG_INF("SDHC");
    } else {
        LOG_INF("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    LOG_INF("SD Card Size: %llu MB\n", cardSize);
}

std::vector<std::string> Flash::getDirectories(const std::string& root)
{

}

void Flash::listDir(const char* dirname)
{
    LOG_INF("Listing directory: %s\n", dirname);

    File root = SD.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
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