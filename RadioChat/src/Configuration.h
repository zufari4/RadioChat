#pragma once

#define DEBUG_MODE 1
#define FIRMWARE_VERSION 0x0001
#define DEVICE_ADDRESS 2
#define SERIAL_SPEED 9600
#define SERIAL_INIT_DELAY 1000
#define STORAGE_DIR "/radio_chat"
#define SETTINGS_FILENAME "config.json"

#define FLASH_PIN_MISO 19
#define FLASH_PIN_MOSI 23
#define FLASH_PIN_SCK 18
#define FLASH_PIN_CS 5

#define KEY_CODE_FN     0x0001
#define KEY_CODE_ENTER  0x000A
#define KEY_CODE_LEFT   0x2190
#define KEY_CODE_UP     0x2191
#define KEY_CODE_RIGHT  0x2192
#define KEY_CODE_DOWN   0x2193
#define KEY_CODE_BACKSPACE 0x0008

#define DISPLAY_FONT u8g2_font_6x12_t_cyrillic
#define LOGGER_DEF_MSG_SIZE 1024
