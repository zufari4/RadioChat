#pragma once

#include "Language.h"
#include <stdint.h>
#include <map>

class KeyMapBase
{
public:
    struct KeyInfo
    {
        uint16_t symbol = '\0';
        uint16_t altSymbol = '\0';
    };

    using tdMapKey = std::map<uint8_t, KeyInfo>;
    
    KeyMapBase(Language lang, const tdMapKey& mapKey);
    virtual ~KeyMapBase();
    uint16_t symbol(uint8_t raw) const;
    uint16_t altSymbol(uint8_t raw) const;
    Language getLang() const;
    
private:
    Language lang_;
    std::map<uint8_t, KeyInfo> mapKey_;
};