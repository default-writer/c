cmake_minimum_required(VERSION 3.22.1)

project(raylib-experiment1 VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(STANDARD
    -save-temps
    -std=gnu89
    -fgnu89-inline
)

set(OPTIONS
    # -Wpedantic
    -Winline
    -Werror
    -Wall
    -Wextra
    -Waggregate-return
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wfloat-equal
    -Wpointer-arith
    -Wshadow
    -Wstrict-overflow=5
    -Wswitch
    -Wswitch-default
    -Wundef
    -Wunreachable-code
    -Wwrite-strings
    -Wformat-nonliteral
    -Wno-absolute-value
    -Wno-builtin-declaration-mismatch
    -Wno-conversion
    -Wno-float-conversion
    -Wno-implicit-function-declaration
    -Wno-implicit-fallthrough
    -Wno-int-conversion
    -Wno-unused-parameter
    -Wno-unused-variable
)

find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0

add_executable(${PROJECT_NAME} examples/ex1/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib m c)

add_compile_options(${STANDARD} ${OPTIONS})
add_link_options(${STANDARD} ${OPTIONS})

include_directories(raylib/src)

link_directories( $ENV{LD_LIBRARY_PATH} )

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
