#include "LogTraceLevel.h"

std::string_view traceLvlToStr(LogTraceLevel level)
{
    switch (level)
    {
    case LogTraceLevel::None:   return "нет";
    case LogTraceLevel::Error:  return "ошибки";
    case LogTraceLevel::Info:   return "инфо";
    case LogTraceLevel::Debug:  return "отладка";
    default:                   return "unknown";
    }
}

std::string_view traceLvlToLog(LogTraceLevel level)
{
    switch (level)
    {
    case LogTraceLevel::None:   return "non";
    case LogTraceLevel::Error:  return "err";
    case LogTraceLevel::Info:   return "inf";
    case LogTraceLevel::Debug:  return "dbg";
    default:                    return "unk";
    }
}
