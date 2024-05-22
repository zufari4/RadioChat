#include "Flash.h"
#include "../Logger/Logger.h"

Flash::Flash()
{
}

Flash::~Flash()
{
}

void Flash::init(const FlashSettings& settings)
{
    LOG_INF("Init flash");
    settings_ = settings;
}