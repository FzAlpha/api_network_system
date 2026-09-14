cmake_minimum_required(VERSION 3.20)

project(CentralPlatform
    VERSION 0.1.0
    DESCRIPTION "Modular API platform scaffold"
    LANGUAGES CXX
)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23 CACHE STRING "C++ standard to use (17, 20, 23)")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include CMake's FetchContent module
include(FetchContent)

# Declare and download Asio (required by Crow)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
)
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
    set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "asio include dir" FORCE)
endif()

# Declare and download Crow from GitHub
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.0 # Latest stable version
)
FetchContent_MakeAvailable(Crow)

# Declare and download SQLiteCpp
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "Run cpplint" FORCE)
set(SQLITECPP_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
set(SQLITECPP_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(SQLITE_ENABLE_COLUMN_METADATA OFF CACHE BOOL "Enable column metadata" FORCE)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG 3.3.1
)
FetchContent_MakeAvailable(SQLiteCpp)
if(NOT MSVC)
    target_compile_options(SQLiteCpp PUBLIC -include cstdint)
endif()

# The repository is currently a structure-first scaffold. This interface
# target gives future libraries, modules, and applications a stable target to
# consume without inventing source files that do not exist yet.
add_library(central_platform INTERFACE)
add_library(CentralPlatform::central_platform ALIAS central_platform)

# Base interface feature set to C++23
target_compile_features(central_platform INTERFACE cxx_std_23)
target_include_directories(central_platform
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
)

# Core error handling library (header-only)
add_library(central_platform_errors INTERFACE)
add_library(CentralPlatform::errors ALIAS central_platform_errors)
target_compile_features(central_platform_errors INTERFACE cxx_std_23)
target_include_directories(central_platform_errors
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core/errors>"
        "$<INSTALL_INTERFACE:include>"
)

# Test executables
add_executable(test_app_error tests/unit/test_app_error.cpp)
target_compile_features(test_app_error PRIVATE cxx_std_23)
target_link_libraries(test_app_error PRIVATE central_platform_errors)

# Examples
add_executable(chat_app examples/chat_app/main.cpp)
target_compile_features(chat_app PRIVATE cxx_std_23)
target_link_libraries(chat_app PRIVATE central_platform central_platform_errors Crow::Crow SQLiteCpp)


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