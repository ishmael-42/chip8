cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

set(PROJECT_NAME chip8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# Don't pull git repo. instead, download source code as zip, and extract it :thumbs:
# https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
    URL_HASH
        SHA256=00e8a7c85f7a5221e56ee8e4e1c6642b773eae73d29379a91c8ac5149e803f42
)

FetchContent_MakeAvailable(raylib)

FetchContent_MakeAvailable(raylib)

set(SOURCE_FILES
    src/main.c
    src/framebuffer.c
    src/gfx.c
)
add_executable(chip8 ${SOURCE_FILES})

target_link_libraries(chip8 PUBLIC raylib)
