cmake_minimum_required(VERSION 3.16.3)

project(list
    VERSION 0.0.1
    LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wno-parentheses -Wno-unused-variable -Wno-unused-parameter -Wno-long-long)

set(MAIN_MICRO_SOURCE tests/main-micro.c)
set(MAIN_TINY_SOURCE tests/main-tiny.c)
set(MAIN_LIGHT_SOURCE tests/main-light.c)
set(MAIN_SOURCE tests/main.c)

add_subdirectory(deps/rexo)

set(LIST_MICRO_SOURCE src/list-micro/code.c)
set(LIST_TINY_SOURCE src/list-tiny/code.c)
set(LIST_LIGHT_SOURCE src/list-light/code.c)
set(LIST_SOURCE src/list/code.c)

include_directories(src/list-micro)
include_directories(src/list-tiny)
include_directories(src/list-light)
include_directories(src/list)
include_directories(src)

add_library(list-micro-lib STATIC ${LIST_MICRO_SOURCE})
add_library(list-tiny-lib STATIC ${LIST_TINY_SOURCE})
add_library(list-light-lib STATIC ${LIST_LIGHT_SOURCE})
add_library(list-lib STATIC ${LIST_SOURCE})

add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-tiny ${MAIN_TINY_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main ${MAIN_SOURCE})

target_link_libraries(main-micro PRIVATE rexo list-micro-lib)
target_link_libraries(main-tiny PRIVATE rexo list-tiny-lib)
target_link_libraries(main-light PRIVATE rexo list-light-lib)
target_link_libraries(main PRIVATE rexo list-lib)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)