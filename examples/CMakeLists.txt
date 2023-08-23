cmake_minimum_required(VERSION 3.22.1)

project(c-oberon VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OBERON 
    oberon/method_suite/figures.c
    oberon/method_suite/rectangles.c
    oberon/method_suite/main.c)

set(OBERON 
    oberon/data_first/figures.c
    oberon/data_first/rectangles.c
    oberon/data_first/main.c)

add_executable(c-oberon-method-suite ${OBERON})
add_executable(c-oberon-data-fist ${OBERON})