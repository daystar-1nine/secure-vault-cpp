cmake_minimum_required(VERSION 3.15)
project(password_manager LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch dependencies
include(FetchContent)

# 1. Fetch webview (Lightweight Web View)
FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview.git
    GIT_TAG master
)
FetchContent_MakeAvailable(webview)

# 2. Fetch Monocypher
FetchContent_Declare(
    monocypher
    GIT_REPOSITORY https://github.com/LoupVaillant/Monocypher.git
    GIT_TAG 4.0.2
)
FetchContent_GetProperties(monocypher)
if(NOT monocypher_POPULATED)
    FetchContent_Populate(monocypher)
endif()

# Define Monocypher source files
set(MONOCYPHER_DIR ${monocypher_SOURCE_DIR})
set(MONOCYPHER_SOURCES
    ${MONOCYPHER_DIR}/src/monocypher.c
)

# Project Source files list
set(SOURCES
    src/main.cpp
    src/auth/AuthManager.cpp
    src/encryption/Encryption.cpp
    src/storage/StorageManager.cpp
    src/vault/VaultManager.cpp
    src/web/webview_impl.cpp
    ${MONOCYPHER_SOURCES}
)

# Define executable (use WIN32 to run as a background windows application with no console window)
add_executable(password_manager WIN32 ${SOURCES})

# Include directories
target_include_directories(password_manager PRIVATE 
    include
    ${MONOCYPHER_DIR}/src
)

# Threading library
find_package(Threads REQUIRED)

# Link libraries (ws2_32 is required on Windows for network sockets)
target_link_libraries(password_manager PRIVATE Threads::Threads webview::core)
if(WIN32)
    target_compile_definitions(password_manager PRIVATE WEBVIEW_EDGE)
    target_link_libraries(password_manager PRIVATE ws2_32 crypt32 ole32 comctl32 oleaut32 uuid shlwapi version)
endif()

# Compiler warning configurations
if(MSVC)
    target_compile_options(password_manager PRIVATE /W3)
else()
    target_compile_options(password_manager PRIVATE -Wall -Wextra)
endif()
