cmake_minimum_required(VERSION 3.20)

project(CentralPlatform
    VERSION 0.1.0
    DESCRIPTION "Modular API platform scaffold"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The repository is currently a structure-first scaffold. This interface
# target gives future libraries, modules, and applications a stable target to
# consume without inventing source files that do not exist yet.
add_library(central_platform INTERFACE)
add_library(CentralPlatform::central_platform ALIAS central_platform)

target_compile_features(central_platform INTERFACE cxx_std_20)
target_include_directories(central_platform
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_target(validate-structure
    COMMAND "${CMAKE_COMMAND}" -E echo
        "CentralPlatform structure is present at ${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Checking the CentralPlatform scaffold"
    VERBATIM
)

include(CTest)
if(BUILD_TESTING)
    add_custom_target(test-structure
        COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target validate-structure
        COMMENT "Running scaffold structure checks"
        VERBATIM
    )
endif()