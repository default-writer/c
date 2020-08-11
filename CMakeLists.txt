cmake_minimum_required(VERSION 3.18)

project(Rexo
    VERSION 0.1.0
    LANGUAGES C CXX)

set(SOURCE src/list.c tests/main.c)
set(HEADER src/list.h)

add_subdirectory(deps/rexo)
add_executable(main tests/main.c)

target_link_libraries(main PRIVATE rexo)

