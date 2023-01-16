cmake_minimum_required(VERSION 3.22.1)

project(list VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MEMORY_DEBUG_INFO "Disables memory debug information")
option(MEMORY_CLEANUP "Disables memory cleanup")
option(MEMORY_ALLOC "Disables memory allocator")

set(MEMORY_DEBUG_INFO ON CACHE BOOL "Enable debug information?")
set(MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(MEMORY_ALLOC OFF CACHE BOOL "Enable memory allocator?")

set(MEMORY_DEBUG_INFO ON)
set(MEMORY_CLEANUP ON)
set(MEMORY_ALLOC OFF)

set(SANITIZE_BUILD_OPTIONS -save-temps -fgnu89-inline -fsanitize=undefined,address,leak -fsanitize=pointer-compare,pointer-subtract -fsanitize-undefined-trap-on-error -fno-sanitize-recover=all)
set(NO_SANITIZE_BUILD_OPTIONS -save-temps -fno-sanitize=all)

if(MEMORY_DEBUG_INFO)
    message(STATUS "Configure with memory debug information")
    add_compile_definitions(MEMORY_DEBUG_INFO)
    add_definitions(-DMEMORY_DEBUG_INFO)
endif()

if(MEMORY_CLEANUP)
    message(STATUS "Configure with memory cleanup")
    add_compile_definitions(MEMORY_CLEANUP)
    add_definitions(-DMEMORY_CLEANUP)
endif()

if(MEMORY_ALLOC)
    message(STATUS "Configure with memory allocator")
    add_compile_definitions(MEMORY_ALLOC)
    add_definitions(-DMEMORY_ALLOC)
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(
        -Winline -Wpedantic -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual
        -Wconversion -Wfloat-equal -Wpointer-arith -Wshadow -Wstrict-overflow=5
        -Wswitch -Wswitch-default -Wundef -Wunreachable-code -Wwrite-strings
        -Wformat-nonliteral -fsanitize=undefined,address)
    add_link_options(
        -fsanitize=undefined,address
    )
endif()

if(UNIX)
    add_definitions(-D_POSIX_C_SOURCE=199309L)
endif()

if(CODE_SANITIZER)
    message("Building with sanitizers")
    add_compile_options(${SANITIZE_BUILD_OPTIONS})
    add_link_options(${SANITIZE_BUILD_OPTIONS})
else()
    message("Building without sanitizers")
    add_compile_options(${NO_SANITIZE_BUILD_OPTIONS})
    add_link_options(${NO_SANITIZE_BUILD_OPTIONS})
endif()

if(CODE_COVERAGE)
    if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        message("Building with llvm code coverage tools")

        # Warning/Error messages
        if(NOT LLVM_COV_PATH)
            message(FATAL_ERROR "llvm-cov not found! Aborting.")
        endif()

        # set Flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

    elseif(CMAKE_COMPILER_IS_GNUCXX)
        message("Building with lcov code coverage tools")

        # Warning/Error messages
        if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
            message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
        endif()
        if(NOT LCOV_PATH)
            message(FATAL_ERROR "lcov not found! Aborting...")
        endif()
        if(NOT GENHTML_PATH)
            message(FATAL_ERROR "genhtml not found! Aborting...")
        endif()

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    else()
        message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
    endif()
    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang" OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-fprofile-arcs -ftest-coverage)
        link_libraries(gcov)
    endif()
endif()

set(LIB_MEMORY_SOURCE src/common/memory.c)
set(LIB_COMMON_SOURCE src/common/alloc.c)
set(LIB_LCG_SOURCE src/common/lcg.c)
set(LIB_PARAMETERS_SOURCE src/common/parameters.c)

set(LIB_SOURCE src/list/code.c)
set(LIB_LIGHT_SOURCE src/list-light/code.c)
set(LIB_MICRO_SOURCE src/list-micro/code.c)
set(LIB_ALLOC_SOURCE src/list-alloc/code.c)
set(LIB_EXPERIMENTAL_SOURCE src/list-experimental/code.c)
set(LIB_PLAYGROUND_SOURCE src/class/code.c)

set(MAIN_SOURCE tests/main.c ${LIB_SOURCE})
set(MAIN_LIGHT_SOURCE tests/main-light.c ${LIB_LIGHT_SOURCE})
set(MAIN_MICRO_SOURCE tests/main-micro.c ${LIB_MICRO_SOURCE})
set(MAIN_ALLOC_SOURCE tests/main-alloc.c ${LIB_ALLOC_SOURCE})
set(MAIN_EXPERIMENTAL_SOURCE tests/main-experimental.c ${LIB_EXPERIMENTAL_SOURCE})
set(MAIN_PLAYGROUND_SOURCE src/playground/structs.c ${LIB_PLAYGROUND_SOURCE})
set(MAIN_MEMORY_SOURCE src/playground/memory.c ${LIB_MEMORY_SOURCE})

include_directories(src)

add_library(list-memory STATIC ${LIB_MEMORY_SOURCE})
add_library(list-common STATIC ${LIB_COMMON_SOURCE})
add_library(list-lcg STATIC ${LIB_LCG_SOURCE})
add_library(list-parameters STATIC ${LIB_PARAMETERS_SOURCE})

add_library(list STATIC ${LIB_SOURCE})
add_library(list-light STATIC ${LIB_LIGHT_SOURCE})
add_library(list-micro STATIC ${LIB_MICRO_SOURCE})
add_library(list-alloc STATIC ${LIB_ALLOC_SOURCE})
add_library(list-experimental STATIC ${LIB_EXPERIMENTAL_SOURCE})
add_library(list-playground STATIC ${LIB_PLAYGROUND_SOURCE})

add_executable(main ${MAIN_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-alloc ${MAIN_ALLOC_SOURCE})
add_executable(main-experimental ${MAIN_EXPERIMENTAL_SOURCE})
add_executable(main-playground ${MAIN_PLAYGROUND_SOURCE})
add_executable(main-memory ${MAIN_MEMORY_SOURCE})

if(CODE_SANITIZER)
    message("Building with sanitizers")
    target_compile_options(main-memory PRIVATE ${SANITIZE_BUILD_OPTIONS})
    target_link_options(main-memory PRIVATE ${SANITIZE_BUILD_OPTIONS})
else()
    message("Building without sanitizers")
    target_compile_options(main-memory PRIVATE ${NO_SANITIZE_BUILD_OPTIONS})
    target_link_options(main-memory PRIVATE ${NO_SANITIZE_BUILD_OPTIONS})
endif()

target_link_libraries(list-alloc PRIVATE list-parameters)
target_link_libraries(list-experimental PRIVATE list-parameters)

target_link_libraries(main PRIVATE list list-common)
target_link_libraries(main-light PRIVATE list-light list-common)
target_link_libraries(main-micro PRIVATE list-micro list-common)
target_link_libraries(main-alloc PRIVATE list-alloc list-lcg list-parameters list-common)
target_link_libraries(main-experimental PRIVATE list-experimental list-lcg list-parameters list-common)
target_link_libraries(main-playground PRIVATE list-playground list-micro list-common)
target_link_libraries(main-memory PRIVATE list-memory)
