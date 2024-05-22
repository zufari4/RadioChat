#include "KeyMapBase.h"

KeyMapBase::KeyMapBase(Language lang, const tdMapKey& mapKey)
    : lang_(lang)
    , mapKey_(mapKey)
{
}

KeyMapBase::~KeyMapBase()
{
}

uint16_t KeyMapBase::symbol(uint8_t raw) const
{
    if (raw == 0) {
        return 0;
    }
    else {
        const auto it = mapKey_.find(raw);
        return it == mapKey_.end() ? 0 : it->second.symbol;
    }
}

uint16_t KeyMapBase::altSymbol(uint8_t raw) const
{
    if (raw == 0) {
        return 0;
    }
    else {
        const auto it = mapKey_.find(raw);
        return it == mapKey_.end() ? 0 : it->second.altSymbol;
    }
}

Language KeyMapBase::getLang() const
{
    return lang_;
}