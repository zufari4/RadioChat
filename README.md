## Description
This repository stores firmware files for a device that allows you to exchange text messages via radio broadcast

## Schema
![alt text](https://github.com/zufari4/RadioChat/blob/main/Docs/Schema.png)

## Hardware used
- **ESP32**
[link](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html)
- **EBYTE E22-400T**
[link](https://www.ebyte.com/en/product-view-news.html?id=939)
- **LCD 12864 (ST7920)**
[link](https://iarduino.ru/shop/displays/lcd-128x64-graficheskiy-siniy-cvet-podsvetki.html)
- **MicroSD Card Adapter**
[link](https://www.ozon.ru/product/modul-b108-dlya-podklyucheniya-micro-sd-karty-spi-readermemory-card-tf-shield-dlya-arduino-982321259/?asb=Rh6z5LeoZrS4GmBfgziU3CqD3tJYN4dWWlDYGVe5RiQ%253D&asb2=PdON14aZ-AFtNI8yl2uKB10MJ7ALf7uiXa4J0hgriuYw7roZgPCGwW5e4p8mQ1Ote-b1M_atFwKUHqFCM7EvUg&avtc=1&avte=2&avts=1720169205&keywords=sd+%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D1%8C+%D0%B0%D1%80%D0%B4%D1%83%D0%B8%D0%BD%D0%BE)
- **Buzzer Module**
- **Voltage Regulator 5V (AMS1117)**
[link](https://www.ozon.ru/product/ldo-ctabilizator-napryazheniya-5v-na-ams1117-836966793)
- **Li-ion Charge Module**
[link](https://www.ozon.ru/product/modul-zaryada-li-ion-akkumulyatorov-type-c-2s-usb-bms-15w-8-4v1-5a-s-balansirovkoy-2-sht-1581768753/)
- **Shift Register**
[link](https://www.chipdip.ru/product/sn74hc165n)

## Prepare for build in Arduino IDE
- Install library u8g2 [link](https://github.com/olikraus/u8g2)
- Install library nlohman-json [link](https://github.com/Johboh/nlohmann-json)
- Add ESP32 board to IDE [link](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) 
