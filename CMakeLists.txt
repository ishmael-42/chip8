cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)

set(PROJECT_NAME chip8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

option(USE_RAYLIB "Use raylib instead of glfw/glad " OFF)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
    URL_HASH
        SHA256=00e8a7c85f7a5221e56ee8e4e1c6642b773eae73d29379a91c8ac5149e803f42
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://codeberg.org/ishmael/glad-33-core-debug.git
    GIT_TAG master
)

if(USE_RAYLIB)
    FetchContent_MakeAvailable(raylib)
    set(gfx_backend raylib)
else()
    FetchContent_MakeAvailable(glfw)
    FetchContent_MakeAvailable(glad)
    set(gfx_backend glad glfw)
endif()

configure_file(src/chip8_config.h.in chip8_config.h)

set(SOURCE_FILES
    src/main.c
    src/framebuffer.c
    src/gfx.c
    src/chip8.c
    src/chip8_instructions.c
    src/window_ctx.c
    src/every.c
    src/chip8_time.c
)
add_executable(chip8 ${SOURCE_FILES})
target_link_libraries(chip8 PUBLIC ${gfx_backend})
target_include_directories(chip8 PRIVATE ${CMAKE_BINARY_DIR})
