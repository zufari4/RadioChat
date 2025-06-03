#include "PropertySection.h"

std::string_view propSectionToStr(PropertySection section)
{
    switch (section)
    {
    case PropertySection::None:     return "None";
    case PropertySection::Log:      return "Log";
    case PropertySection::Esp:      return "ESP";
    case PropertySection::Keyboard: return "Keyboard";
    case PropertySection::Display:  return "Display";
    case PropertySection::Battery:  return "Battery";
    case PropertySection::Contacts: return "Contacts";
    case PropertySection::Flash:    return "Flash";
    case PropertySection::Led:      return "Led";
    case PropertySection::Radio:    return "Radio";
    case PropertySection::Sound:    return "Sound";
    case PropertySection::UI:       return "UI";
    default:                        return "Unknown";
    }
}
