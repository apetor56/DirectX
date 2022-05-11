cmake_minimum_required(VERSION 3.0.0)
project("keyboard and mouse input" VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_compile_options(-std=c++17 -Wall -pedantic)

# set static library preffix and suffix and set hint to find libraries
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
set(CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x64")

# set source files path
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# set include file path
set(INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include)

message("Start building target")


find_library(D3D11
    d3d11.lib
    HINTS ${CMAKE_PREFIX_PATH}
)
message(${D3D11})

# create the exe file
add_executable(WinMain
    ${SRC}/WinMain.cpp
    ${SRC}/Window.cpp
    ${SRC}/DXException.cpp
    ${SRC}/Keyboard.cpp
    ${SRC}/Mouse.cpp
    ${SRC}/Timer.cpp
    ${SRC}/App.cpp
    ${SRC}/Graphics.cpp
)

# add to include path to WinMain target,
# always after add_executable/add_library
target_include_directories(WinMain PRIVATE ${INCLUDE_FILES})
target_link_libraries(WinMain PRIVATE ${D3D11})