#pragma once

#define DEBUG_MODE 1
#define SERIAL_BAUDRATE 9600
#define SETTINGS_FILENAME "config.json"
#define COUNT_KEYBOUARD_REGISTERS 5

#define KEY_CODE_FN     0x0001
#define KEY_CODE_ENTER  0x000A
#define KEY_CODE_LEFT   0x2190
#define KEY_CODE_UP     0x2191
#define KEY_CODE_RIGHT  0x2192
#define KEY_CODE_DOWN   0x2193
#define KEY_CODE_BACKSPACE 0x0008

#define DISPLAY_FONT u8g2_font_6x12_t_cyrillic