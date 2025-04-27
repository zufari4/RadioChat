set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER xtensa-esp32-elf-gcc)
set(CMAKE_CXX_COMPILER xtensa-esp32-elf-g++)
set(CMAKE_ASM_COMPILER xtensa-esp32-elf-gcc)
set(CMAKE_AR xtensa-esp32-elf-ar)
set(CMAKE_OBJCOPY xtensa-esp32-elf-objcopy)
set(CMAKE_OBJDUMP xtensa-esp32-elf-objdump)

set(ESP32_PATH "$ENV{LocalAppData}/Arduino15/packages/esp32/hardware/esp32/3.2.0")
set(ESP32_IDF "$ENV{LocalAppData}/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.4-2f7dcd86-v1") 

set(COMMON_FLAGS "-mlongcalls -Wno-frame-address")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}" CACHE STRING "C Flags")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}" CACHE STRING "C++ Flags")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--gc-sections" CACHE STRING "Linker Flags")

include_directories(
    "${ESP32_PATH}/libraries/SD/src"
    "${ESP32_PATH}/libraries/FS/src"
	"${ESP32_PATH}/libraries/SPI/src"
	"${ESP32_PATH}/libraries/WiFi/src"
	"${ESP32_PATH}/cores/esp32"
	"${ESP32_IDF}/esp32/include/soc/esp32/include"
	"${ESP32_IDF}/esp32/include/esp_wifi/include"
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)