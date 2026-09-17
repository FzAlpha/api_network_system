cmake_minimum_required(VERSION 3.20)

project(ApiNetworkSystem
    VERSION 0.1.0
    DESCRIPTION "API Network System"
    LANGUAGES CXX
)

# Configurable C++ standard (supports 17, 20, 23; defaults to 23)
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23 CACHE STRING "C++ standard to use (17, 20, 23)")
    set_property(CACHE CMAKE_CXX_STANDARD PROPERTY STRINGS "17" "20" "23")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Cross-platform compiler flags (Linux, macOS, Windows with GCC, Clang, AppleClang, MSVC)
if(MSVC)
    add_compile_options(/utf-8 /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(CTest)

add_subdirectory(CentralPlatform)

