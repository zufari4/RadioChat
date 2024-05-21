h1. Radio module for text chat

h3. Hardware

# ESP32
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html

h3. Build in arduino IDE

# place file platform.local.txt
to AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.16\
<pre>
compiler.c.extra_flags=-std=gnu17
compiler.cpp.extra_flags=-std=gnu++17
<pre>