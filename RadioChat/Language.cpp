#include "Language.h"

const char* lang2str(Language lang)
{
    switch (lang)
    {
    case Language::Russian: return "Russian";
    case Language::English: return "English";
    default: return "Unknown";
    }
}