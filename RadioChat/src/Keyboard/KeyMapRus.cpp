#include "KeyMapRus.h"
#include "../Configuration.h"

// raw
// [ 1] [ 7] [ 3] [10] [14] [12] [23] [19] [25] [31] [27] [39]
//    [ 8] [ 5] [ 9] [15] [11] [18] [22] [20] [32] [29] [40]
// [ 2] [ 6] [ 4] [16] [13] [17] [24] [21] [26] [30] [28] [38]

// code
// [ й] [ ц] [ у] [ к] [ е] [ н] [ г] [ ш] [ щ] [ з] [ х] [ ъ]
//    [ ф] [ ы] [ в] [ а] [ п] [ р] [ о] [ л] [ д] [ ж] [ э]
// [fn] [ я] [ ч] [ с] [ м] [ и] [ т] [ ь] [ б] [ ю] [  ] [ ⏎]

// alt
// [ 1] [ 2] [ 3] [ 4] [ 5] [ 6] [ 7] [ 8] [ 9] [ 0] [ -] [bk]
//    [ "] [ ↑] [ ~] [ /] [ %] [ @] [ =] [ |] [ \] [ :] [ +]
// [fn] [ ←] [ ↓] [ →] [ #] [ &] [ !] [ *] [ ,] [ .] [ ?] [ ⏎]

KeyMapRus::KeyMapRus()
    : KeyMapBase(Language::Russian, 
    {
        {  1, {0xD0B9, '1'} },                // й
        {  7, {0xd186, '2'} },                // ц
        {  3, {0xd183, '3'} },                // у
        { 10, {0xd0ba, '4'} },                // к
        { 14, {0xd0b5, '5'} },                // е
        { 12, {0xd0bd, '6'} },                // н
        { 23, {0xd0b3, '7'} },                // г
        { 19, {0xd188, '8'} },                // ш
        { 25, {0xd189, '9'} },                // щ
        { 31, {0xd0b7, '0'} },                // з
        { 27, {0xd185, '-'} },                // х
        { 39, {0xd18a, KEY_CODE_BACKSPACE} }, // ъ

        {  8, {0xd184, '"'} },                // ф
        {  5, {0xd18b, KEY_CODE_UP} },        // ы    
        {  9, {0xd0b2, '~'} },                // в        
        { 15, {0xd0b0, '/'} },                // а        
        { 11, {0xd0bf, '%'} },                // п        
        { 18, {0xd180, '@'} },                // р        
        { 22, {0xd0be, '='} },                // о        
        { 20, {0xd0bb, '|'} },                // л        
        { 32, {0xd0b4, '\\'} },               // д
        { 29, {0xd0b6, ':'} },                // ж
        { 40, {0xd18d, '+'} },                // э

        {  2, {KEY_CODE_FN, KEY_CODE_FN} },                        
        {  6, {0xd18f, KEY_CODE_LEFT} },      // я                         
        {  4, {0xd187, KEY_CODE_DOWN} },      // ч                         
        { 16, {0xd181, KEY_CODE_RIGHT} },     // с                             
        { 13, {0xd0bc, '#'} },                // м                         
        { 17, {0xd0b8, '&'} },                // и                         
        { 24, {0xd182, '!'} },                // т                         
        { 21, {0xd18c, '*'} },                // ь                         
        { 26, {0xd0b1, ','} },                // б                         
        { 30, {0xd18e, '.'} },                // ю                         
        { 28, {' ', '?'} },  
        { 38, {KEY_CODE_ENTER, KEY_CODE_ENTER} },      
    })
{
}

KeyMapRus::~KeyMapRus()
{
}