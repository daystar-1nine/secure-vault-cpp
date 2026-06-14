cmake_minimum_required(VERSION 3.15)
project(password_manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch dependencies
include(FetchContent)

# 1. Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(glfw)

# 2. Fetch Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# Define ImGui source files (compiling them directly)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

# Project Source files list
set(SOURCES
    src/main.cpp
    src/auth/AuthManager.cpp
    src/encryption/Encryption.cpp
    src/storage/StorageManager.cpp
    src/vault/VaultManager.cpp
    ${IMGUI_SOURCES}
)

# Define executable (use WIN32 to hide terminal window on Windows)
add_executable(password_manager WIN32 ${SOURCES})

# Include directories
target_include_directories(password_manager PRIVATE 
    include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link GLFW and system OpenGL libraries
target_link_libraries(password_manager PRIVATE glfw)
if(WIN32)
    target_link_libraries(password_manager PRIVATE opengl32 gdi32)
else()
    target_link_libraries(password_manager PRIVATE OpenGL)
endif()

# Compiler warning configurations (avoid Werror to prevent external dependency warnings from stopping the build)
if(MSVC)
    target_compile_options(password_manager PRIVATE /W3)
else()
    target_compile_options(password_manager PRIVATE -Wall -Wextra)
endif()
