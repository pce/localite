cmake_minimum_required(VERSION 3.22)

project(
    "Localite"
    VERSION 1.0.0
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)
option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)

# include(Warnings)
#  include(Tools) # needs clang tidy and clang format
# include(ConfigSafeGuards)

# add_clang_format_target() # needs python


# for universal binary
# cmake -DCMAKE_OSX_ARCHITECTURES=x86_64;arm64 ..
# cmake -DCMAKE_OSX_ARCHITECTURES=arm64 ..
set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "Target architectures" FORCE)


# you can set any other vcpkg installation on your system
set (VCPKG_DIR ${CMAKE_SOURCE_DIR}/../../external/vcpkg)
# include(${VCPKG_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
include(${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Add the directory containing stb_image.h to the include path
include_directories(external) 
# Add json
# include_directories(${PROJECT_SOURCE_DIR}/external/json/include)
include_directories(external/json/include)


add_executable(${PROJECT_NAME}
    src/main.cpp
    src/ContentBufferView.cpp
    src/FileUtils.cpp
    src/MediaPreview.cpp
)

add_compile_definitions(NOMINMAX)

#target_set_warnings(TARGET ${PROJECT_NAME}
#    ENABLE ${ENABLE_WARNINGS}
#    AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
# add_clang_tidy_msvc_to_target(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${Stb_INCLUDE_DIR} src/)

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)
