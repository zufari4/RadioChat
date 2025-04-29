cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(RadioChat)

set(CMAKE_CXX_STANDARD 17)
include(esp32_toolchain.cmake)
include_directories(${CMAKE_SOURCE_DIR}/RadioChat/src)
include_directories("c:/Dev/Arduino/libraries/U8g2/src")
include_directories("c:/Dev/Arduino/libraries/nlohmann-json/src")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/RadioChat/src/*.h" "${CMAKE_SOURCE_DIR}/RadioChat/src/*.cpp")
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE
    U8X8_USE_PINS
    SOC_BT_SUPPORTED
    SOC_WIFI_SUPPORTED
    F_CPU=240000000L
    ARDUINO=10607
    ARDUINO_ESP32_DEV
    ARDUINO_ARCH_ESP32
    ARDUINO_BOARD="ESP32_DEV"
    ARDUINO_VARIANT="esp32"
    ARDUINO_PARTITION_huge_app
    ARDUINO_HOST_OS="windows"
    ARDUINO_FQBN="esp32:esp32:esp32:UploadSpeed=921600,CPUFreq=240,FlashFreq=80,FlashMode=qio,FlashSize=4M,PartitionScheme=huge_app,DebugLevel=verbose,PSRAM=disabled,LoopCore=1,EventsCore=1,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default"
    ESP32=ESP32
    CORE_DEBUG_LEVEL=5
    ARDUINO_RUNNING_CORE=1
    ARDUINO_EVENT_RUNNING_CORE=1
    ARDUINO_USB_CDC_ON_BOOT=0
)
