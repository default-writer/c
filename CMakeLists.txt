cmake_minimum_required(VERSION 3.22.1)

project(list VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_MICRO_SOURCE src/list-micro/code.c src/common/alloc.c src/common/print.c src/common/object.c)
set(LIB_LIGHT_SOURCE src/list-light/code.c src/common/alloc.c src/common/print.c src/common/object.c)
set(LIB_SOURCE src/list/code.c src/common/alloc.c src/common/print.c src/common/object.c)
set(LIB_EXPERIMENTAL_SOURCE src/list-experimental/code.c src/common/alloc.c)
set(LIB_ALLOC_SOURCE src/list-alloc/code.c src/common/alloc.c)
set(LIB_PLAYGROUND_SOURCE src/classes/code.c src/common/alloc.c)

set(MAIN_MICRO_SOURCE tests/main-micro.c ${LIB_MICRO_SOURCE})
set(MAIN_LIGHT_SOURCE tests/main-light.c ${LIB_LIGHT_SOURCE})
set(MAIN_SOURCE tests/main.c ${LIB_SOURCE})
set(MAIN_EXPERIMENTAL_SOURCE tests/main-experimental.c ${LIB_EXPERIMENTAL_SOURCE})
set(MAIN_ALLOC_SOURCE tests/main-alloc.c ${LIB_ALLOC_SOURCE})
set(MAIN_PLAYGROUND_SOURCE playground/structs.c ${LIB_PLAYGROUND_SOURCE})

option(MEMORY_DEBUG_INFO "Disables memory allocation debug information")
option(MEMORY_CLEANUP "Disables memory cleanup")
option(MEMORY_LEAKS "Disables memory leaks detection")

set(MEMORY_DEBUG_INFO ON CACHE BOOL "Enable memory allocation debug information?")
set(MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(MEMORY_LEAKS ON CACHE BOOL "Enable memory leaks detection?")

set(MEMORY_DEBUG_INFO ON)
set(MEMORY_CLEANUP ON)
set(MEMORY_LEAKS OFF)

if(MEMORY_DEBUG_INFO)
    message(STATUS "Configure with memory allocation debug information")
    add_compile_definitions(MEMORY_DEBUG_INFO)
    add_definitions(-DMEMORY_DEBUG_INFO)
endif()

if(MEMORY_CLEANUP)
    message(STATUS "Configure with memory cleanup")
    add_compile_definitions(MEMORY_CLEANUP)
    add_definitions(-DMEMORY_CLEANUP)
endif()

if(MEMORY_LEAKS)
    message(STATUS "Configure with memory leaks detection")
    add_compile_definitions(MEMORY_LEAKS)
    add_definitions(-DMEMORY_LEAKS)
endif()

include_directories(src)

add_compile_options(-fsanitize=undefined,address -fsanitize-undefined-trap-on-error -fno-sanitize-recover=all)
add_link_options(-fsanitize=undefined,address -fsanitize-undefined-trap-on-error -fno-sanitize-recover=all)

add_library(liblist-micro STATIC ${LIB_MICRO_SOURCE})
add_library(liblist-light STATIC ${LIB_LIGHT_SOURCE})
add_library(liblist STATIC ${LIB_SOURCE})
add_library(liblist-experimental STATIC ${LIB_EXPERIMENTAL_SOURCE})
add_library(liblist-alloc STATIC ${LIB_ALLOC_SOURCE})
add_library(liblist-playground STATIC ${LIB_PLAYGROUND_SOURCE})

add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main ${MAIN_SOURCE})
add_executable(main-experimental ${MAIN_EXPERIMENTAL_SOURCE})
add_executable(main-alloc ${MAIN_ALLOC_SOURCE})
add_executable(main-playground ${MAIN_PLAYGROUND_SOURCE})

target_link_libraries(main-micro PRIVATE liblist-micro)
target_link_libraries(main-light PRIVATE liblist-light)
target_link_libraries(main PRIVATE liblist)
target_link_libraries(main-experimental PRIVATE liblist-experimental)
target_link_libraries(main-alloc PRIVATE liblist-alloc)
target_link_libraries(main-playground PRIVATE liblist-playground)

if(CMAKE_BUILD_TYPE STREQUAL "coverage" OR CODE_COVERAGE)
    if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        message("Building with llvm Code Coverage Tools")

        # Warning/Error messages
        if(NOT LLVM_COV_PATH)
            message(FATAL_ERROR "llvm-cov not found! Aborting.")
        endif()

        # set Flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

    elseif(CMAKE_COMPILER_IS_GNUCXX)
        message("Building with lcov Code Coverage Tools")

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
endif()

if(CODE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang"
    OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fprofile-arcs -ftest-coverage)
    link_libraries(gcov)
    endif()
endif()