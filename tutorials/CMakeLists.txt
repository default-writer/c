cmake_minimum_required(VERSION 3.22.1)

project(c-oberon VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)

set(LESSON1 src/lesson-01/main.c)
set(LESSON2 src/lesson-02/main.c)
set(LESSON3 src/lesson-03/main.c)
set(LESSON4 src/lesson-04/main.c)

add_executable(c-lesson-01 ${LESSON1})
add_executable(c-lesson-02 ${LESSON2})
add_executable(c-lesson-03 ${LESSON3})
add_executable(c-lesson-04 ${LESSON4})