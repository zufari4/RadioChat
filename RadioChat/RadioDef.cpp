#include "RadioDef.h"

namespace RadioDef
{

const char* radio_mode_str(RadioMode mode)
{
    switch (mode)
    {
    case RadioMode::Undefined: return "Undefined";
	case RadioMode::Transfer: return "Transfer";
	case RadioMode::WOR: return "WOR";
	case RadioMode::Configuration: return "Configuration";
	case RadioMode::Sleep: return "Sleep";
    default: return "Unknown";
    }
}

} // namespace RadioDef