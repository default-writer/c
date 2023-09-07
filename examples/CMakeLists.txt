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
    oberon/method_suite/object/object.c
    oberon/method_suite/figure/figure.c
    oberon/method_suite/rectangle/rectangle.c
    oberon/method_suite/main.c)
set(SIMPLER_PARSER
    strings/simple_parser.c)

add_executable(c-oberon-method-suite ${OBERON})
add_executable(c-simple-parser ${SIMPLER_PARSER})