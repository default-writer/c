cmake_minimum_required(VERSION 3.18)

project(main
    VERSION 0.0.1
    LANGUAGES C CXX)

set(SOURCE tests/main.c)
set(HEADER src/list.h)

include_directories(src)

add_subdirectory(deps/rexo)
add_executable(main ${SOURCE})

add_subdirectory(src)

target_link_libraries(main PRIVATE rexo list)
