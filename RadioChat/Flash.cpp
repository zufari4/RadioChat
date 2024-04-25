#include "Flash.h"
#include "Logger.h"

Flash::Flash()
{
}

Flash::~Flash()
{
}

void Flash::init(const FlashSettings& settings)
{
    LOG_INF("Init flash\n");
    settings_ = settings;
}