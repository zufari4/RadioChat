set(ESP32_PATH "$ENV{LocalAppData}/Arduino15/packages/esp32/hardware/esp32/3.2.0")
set(ESP32_IDF "$ENV{LocalAppData}/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.4-2f7dcd86-v1") 
set(ESP32_BIN "$ENV{LocalAppData}/Arduino15/packages/esp32/tools/esp-x32/2411/bin")

set(CMAKE_C_COMPILER "${ESP32_BIN}/xtensa-esp32-elf-gcc")
set(CMAKE_CXX_COMPILER "${ESP32_BIN}/xtensa-esp32-elf-g++")
set(CMAKE_ASM_COMPILER "${ESP32_BIN}/xtensa-esp32-elf-gcc")
set(CMAKE_AR "${ESP32_BIN}/xtensa-esp32-elf-ar")
set(CMAKE_OBJCOPY "${ESP32_BIN}/xtensa-esp32-elf-objcopy")
set(CMAKE_OBJDUMP "${ESP32_BIN}/xtensa-esp32-elf-objdump")

set(CMAKE_CXX_FLAGS "-std=c17" CACHE STRING "C++ Compiler Flags" FORCE)
set(CMAKE_CXX_FLAGS "-std=c++17" CACHE STRING "C++ Compiler Flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--gc-sections" CACHE STRING "Linker Flags" FORCE)

include_directories(
    "${ESP32_PATH}/libraries/SD/src"
    "${ESP32_PATH}/libraries/FS/src"
	"${ESP32_PATH}/libraries/SPI/src"
	"${ESP32_PATH}/libraries/WiFi/src"
	"${ESP32_PATH}/cores/esp32"
	"${ESP32_PATH}/variants/esp32"
	"${ESP32_IDF}/esp32/include/soc/esp32/include"
	"${ESP32_IDF}/esp32/include/esp_wifi/include"
	"${ESP32_IDF}/esp32/include"
	"${ESP32_IDF}/esp32/qio_qspi/include"
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)