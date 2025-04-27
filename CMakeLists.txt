cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(RadioChat)

set(CMAKE_CXX_STANDARD 17)
include(esp32_toolchain.cmake)
include_directories(${CMAKE_SOURCE_DIR}/RadioChat/src)
include_directories("c:/Dev/Arduino/libraries/U8g2/src")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/RadioChat/src/*.h" "${CMAKE_SOURCE_DIR}/RadioChat/src/*.cpp")
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE U8X8_USE_PINS SOC_BT_SUPPORTED SOC_WIFI_SUPPORTED)
target_link_libraries(${PROJECT_NAME}
    -lc
    -lstdc++
    -lgcc
    -lapp_update
    -lspi_flash
)
