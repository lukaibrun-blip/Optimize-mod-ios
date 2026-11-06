cmake_minimum_required(VERSION 3.21)
project(ios-optimization VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

if (NOT DEFINED ENV{GEODE_SDK_PATH})
    message(FATAL_ERROR "GEODE_SDK_PATH não foi definido. Verifique se o build-geode-mod usa target: iOS.")
endif()

add_subdirectory($ENV{GEODE_SDK_PATH} ${CMAKE_BINARY_DIR}/geode)
setup_geode_mod(${PROJECT_NAME})
