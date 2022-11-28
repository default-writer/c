cmake_minimum_required(VERSION 3.22.1)

project(list VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MAIN_MICRO_SOURCE tests/main-micro.c src/common/alloc.c src/common/print.c src/common/object.c)
set(MAIN_TINY_SOURCE tests/main-tiny.c src/common/alloc.c src/common/print.c src/common/object.c)
set(MAIN_LIGHT_SOURCE tests/main-light.c src/common/alloc.c src/common/print.c src/common/object.c)
set(MAIN_SOURCE tests/main.c src/common/alloc.c src/common/print.c src/common/object.c)

set(LIST_MICRO_SOURCE src/list-micro/code.c src/common/alloc.c src/common/print.c src/common/object.c)
set(LIST_TINY_SOURCE src/list-tiny/code.c src/common/alloc.c src/common/print.c src/common/object.c)
set(LIST_LIGHT_SOURCE src/list-light/code.c src/common/alloc.c src/common/print.c src/common/object.c)
set(LIST_SOURCE src/list/code.c src/common/alloc.c src/common/print.c src/common/object.c)

option(MEMORY_DEBUG_INFO "Disables memory allocation debug information")
option(MEMORY_CLEANUP "Disables memory cleanup")
option(MEMORY_LEAKS "Disables memory leaks detection")

set(MEMORY_DEBUG_INFO ON CACHE BOOL "Disable memory allocation debug information?")
set(MEMORY_CLEANUP ON CACHE BOOL "Disable memory cleanup?")
set(MEMORY_LEAKS ON CACHE BOOL "Disable memory leaks detection?")

set(MEMORY_DEBUG_INFO OFF)
set(MEMORY_CLEANUP OFF)
set(MEMORY_LEAKS OFF)

if(MEMORY_DEBUG_INFO)
    message(STATUS "Configure without memory allocation debug information")
    add_compile_definitions(MEMORY_DEBUG_INFO)
    add_definitions(-DMEMORY_DEBUG_INFO)
endif()

if(MEMORY_CLEANUP)
    message(STATUS "Configure without memory cleanup")
    add_compile_definitions(MEMORY_CLEANUP)
    add_definitions(-DMEMORY_CLEANUP)
endif()

if(MEMORY_LEAKS)
    message(STATUS "Configure without memory leaks detection")
    add_compile_definitions(MEMORY_LEAKS)
    add_definitions(-DMEMORY_LEAKS)
endif()

include_directories(src)
include_directories(src/std)
include_directories(src/common)
include_directories(src/list-micro)
include_directories(src/list-tiny)
include_directories(src/list-light)
include_directories(src/list)

add_compile_options(-Wno-format-extra-args -Wno-format -Wno-parentheses -Wno-unused-variable -Wno-unused-parameter -Wno-long-long -fsanitize=undefined,address)
add_link_options(-fsanitize=undefined,address)

add_library(list-micro-lib STATIC ${LIST_MICRO_SOURCE})
add_library(list-tiny-lib STATIC ${LIST_TINY_SOURCE})
add_library(list-light-lib STATIC ${LIST_LIGHT_SOURCE})
add_library(list-lib STATIC ${LIST_SOURCE})

add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-tiny ${MAIN_TINY_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main ${MAIN_SOURCE})

target_link_libraries(main-micro PRIVATE list-micro-lib)
target_link_libraries(main-tiny PRIVATE list-tiny-lib)
target_link_libraries(main-light PRIVATE list-light-lib)
target_link_libraries(main PRIVATE list-lib)
