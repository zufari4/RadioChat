#include "Melody.h"
#include "pitches.h"

namespace Melody
{

Info packman()
{
    const Info melody {
        Name::Packman,
        "Packman",
        {
        {NOTE_B4, 16}, {NOTE_B5 ,  16}, {NOTE_FS5, 16}, {NOTE_DS5, 16}, //1
        {NOTE_B5, 32}, {NOTE_FS5, -16}, {NOTE_DS5,  8}, {NOTE_C5,  16},
        {NOTE_C6, 16}, {NOTE_G6 ,  16}, {NOTE_E6 , 16}, {NOTE_C6,  32}, {NOTE_G6, -16}, {NOTE_E6, 8},

        {NOTE_B4 ,  16}, {NOTE_B5 , 16}, {NOTE_FS5, 16}, {NOTE_DS5, 16}, {NOTE_B5 , 32},  //2
        {NOTE_FS5, -16}, {NOTE_DS5,  8}, {NOTE_DS5, 32}, {NOTE_E5 , 32}, {NOTE_F5 , 32},
        {NOTE_F5 ,  32}, {NOTE_FS5, 32}, {NOTE_G5 , 32}, {NOTE_G5 , 32}, {NOTE_GS5, 32}, {NOTE_A5, 16}, {NOTE_B5, 8}
        }
    };
    return melody;
}

Info packmanShort()
{
    const Info melody {
        Name::PackmanShort,
        "PackmanShort",
        {
        {NOTE_B4, 16}, {NOTE_B5 ,  16}, {NOTE_FS5, 16}, {NOTE_DS5, 16}
        }
    };
    return melody;
}

Info nokia()
{
    const Info melody {
        Name::Nokia,
        "Nokia",
        {
        {NOTE_E5 , 8}, {NOTE_D5, 8}, {NOTE_FS4, 4}, {NOTE_GS4, 4}, 
        {NOTE_CS5, 8}, {NOTE_B4, 8}, {NOTE_D4 , 4}, {NOTE_E4 , 4}, 
        {NOTE_B4 , 8}, {NOTE_A4, 8}, {NOTE_CS4, 4}, {NOTE_E4 , 4},
        {NOTE_A4 , 2} 
        }
    };
    return melody;
}

Info acceptMld()
{
    const Info melody{
        Name::Accept,
        "accept",
        {
        {NOTE_GS5, 32}, {NOTE_A5, 16}, {NOTE_B5, 8}
        }
    };
    return melody;
}

Info getMelody(Name melodyName)
{
    switch (melodyName)
    {   
    case Name::Packman: return packman();
    case Name::PackmanShort: return packmanShort();
    case Name::Nokia: return nokia();
    case Name::Accept: return acceptMld();
    default: return packman();
    }
}

}