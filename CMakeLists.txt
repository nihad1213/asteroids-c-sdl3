cmake_minimum_required(VERSION 3.31)
project(asteroids-c-sdl3 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_TTF REQUIRED IMPORTED_TARGET sdl3-ttf)

file(GLOB SRCS "src/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

add_executable(game ${SRCS})

target_include_directories(game PRIVATE include)
target_link_libraries(game
    PRIVATE
        PkgConfig::SDL3
        PkgConfig::SDL3_IMAGE
        PkgConfig::SDL3_TTF
        m
)

add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game
    DEPENDS game
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running game..."
)