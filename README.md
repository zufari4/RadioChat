# Radio module for text chat

## Hardware

### ESP32

[Discription Page](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html)

## Build in arduino IDE

- place file platform.local.txt
to AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.16\
```
compiler.c.extra_flags=-std=gnu17
compiler.cpp.extra_flags=-std=gnu++17
```