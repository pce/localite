cmake_minimum_required(VERSION 3.22)

project(
        "Localite"
        VERSION 1.0.0
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Universal binary for macOS
set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "Target architectures" FORCE)

# Allow vcpkg to be used for dependency management
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE is not defined! Please ensure you're using vcpkg as the toolchain.")
endif()

# Ensure vcpkg toolchain is set only on Windows
if (WIN32)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Manually add external directories
include_directories(external) # External directory for stb_image.h
include_directories(${PROJECT_SOURCE_DIR}/external/json/include)  # Manually included JSON headers, move to vcpkg?

# Dependencies from vcpkg
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
# find_package(nlohmann_json CONFIG REQUIRED)

# Find glad only on Windows
#if (WIN32)
#    find_package(glad CONFIG REQUIRED)
#endif()

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/ContentBufferView.cpp
        src/FileUtils.cpp
        src/MediaPreview.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC src/)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)
# target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)  # Link nlohmann_json
# Link glad only on Windows
if (WIN32)
    find_package(glad CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
endif()
