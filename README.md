# Radio module for text chat

## Hardware

- **ESP32**
[Discription Page](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html)

- **EBYTE E22-400T**

## Build in arduino IDE

- Create file platform.local.txt
to %localappdata%\Arduino15\packages\esp32\hardware\esp32\2.0.16\
```
compiler.c.extra_flags=-std=gnu17
compiler.cpp.extra_flags=-std=gnu++17
```
- clean cache
```%localappdata%\Temp\```
