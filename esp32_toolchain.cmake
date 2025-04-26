# Указываем на кросс-компилятор ESP32
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER xtensa-esp32-elf-gcc)
set(CMAKE_CXX_COMPILER xtensa-esp32-elf-g++)
set(CMAKE_ASM_COMPILER xtensa-esp32-elf-gcc)
set(CMAKE_AR xtensa-esp32-elf-ar)
set(CMAKE_OBJCOPY xtensa-esp32-elf-objcopy)
set(CMAKE_OBJDUMP xtensa-esp32-elf-objdump)

# Пути к ESP-IDF (замените на свой путь!)
set(ESP32_COMPONENTS_PATH "$ENV{LocalAppData}/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.1-442a798083")

# Флаги компиляции для ESP32
set(COMMON_FLAGS "-mlongcalls -Wno-frame-address")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}" CACHE STRING "C Flags")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}" CACHE STRING "C++ Flags")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--gc-sections" CACHE STRING "Linker Flags")

# Пути к заголовочным файлам и библиотекам ESP-IDF
include_directories(
    "${ESP32_COMPONENTS_PATH}/esp32/include"
    "${ESP32_COMPONENTS_PATH}/driver/include"
    "${ESP32_COMPONENTS_PATH}/freertos/include"
    # Добавьте другие необходимые пути
)

# Отключаем поиск программ в стандартных путях
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)