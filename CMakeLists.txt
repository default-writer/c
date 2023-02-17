cmake_minimum_required(VERSION 3.22.1)

project(list VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CONFIG_MEMORY_DEBUG_INFO "Enables memory debug information")
option(CONFIG_MEMORY_CLEANUP "Enables memory cleanup")
option(CONFIG_MEMORY_ALLOC "Enables memory allocator")

set(CONFIG_MEMORY_DEBUG_INFO ON CACHE BOOL "Enable debug information?")
set(CONFIG_MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(CONFIG_MEMORY_ALLOC OFF CACHE BOOL "Enable memory allocator?")

set(CONFIG_MEMORY_DEBUG_INFO ON)
set(CONFIG_MEMORY_CLEANUP ON)
set(CONFIG_MEMORY_ALLOC OFF)

set(STANDARD
    -save-temps
    -std=gnu89
    -fgnu89-inline
)

set(INLINE 
    -finline-functions-called-once
)

set(CLANGD
    -fsanitize=bounds-strict
    -static-libasan
)

set(SANITIZE
    -fsanitize=address,undefined,leak,pointer-compare,pointer-subtract,float-cast-overflow,float-divide-by-zero
    -fsanitize-undefined-trap-on-error
    -fno-sanitize-recover=all
)

set(NO_SANITIZE
    -fno-sanitize=all
)

set(OPTIONS
    -Winline
    -Wpedantic
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
)

set(STRICT
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wold-style-definition
)

if(CONFIG_MEMORY_DEBUG_INFO)
    message(STATUS "Configure with memory debug information")
    add_compile_definitions(CONFIG_MEMORY_DEBUG_INFO)
    add_definitions(-DCONFIG_MEMORY_DEBUG_INFO)
endif()

if(CONFIG_MEMORY_CLEANUP)
    message(STATUS "Configure with memory cleanup")
    add_compile_definitions(CONFIG_MEMORY_CLEANUP)
    add_definitions(-DCONFIG_MEMORY_CLEANUP)
endif()

if(CONFIG_MEMORY_ALLOC)
    message(STATUS "Configure with memory allocator")
    add_compile_definitions(CONFIG_MEMORY_ALLOC)
    add_definitions(-DCONFIG_MEMORY_ALLOC)
endif()

if(CODE_SANITIZER)
    message("Building with sanitizers")
    add_compile_options(${STANDARD} ${SANITIZE} ${OPTIONS})
    add_link_options(${STANDARD} ${SANITIZE} ${OPTIONS})
else()
    message("Building without sanitizers")
    add_compile_options(${STANDARD} ${NO_SANITIZE})
    add_link_options(${STANDARD} ${NO_SANITIZE})
endif()

if(MOCKS)
    message("Building with mocks")
    add_compile_definitions(MOCKS)
    add_definitions(-DMOCKS)
else()
    message("Building without mocks")
endif()

if(GC)
    message("Building with garbage collector")
    add_compile_definitions(GC)
    add_definitions(-DGC)
else()
    message("Building without garbage collector")
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

if(UNIX)
    add_definitions(-D_POSIX_C_SOURCE=199309L)
endif()

set(LIB_COMMON_SOURCE src/common/alloc.c)
set(LIB_LCG_SOURCE src/common/lcg.c)
set(LIB_PARAMETERS_SOURCE src/common/parameters.c)

set(LIB_LIST1_SOURCE src/playground/list/v1/list.c)
set(LIB_LIST2_SOURCE src/playground/list/v2/list.c)
set(LIB_VM_SOURCE src/playground/virtual/vm.c)
set(LIB_HASHTABLE_SOURCE src/playground/hashtable/hashtable.c)
set(LIB_STANDARD_SOURCE src/list/code.c)
set(LIB_LIGHT_SOURCE src/list-light/code.c)
set(LIB_MICRO_SOURCE src/list-micro/code.c)
set(LIB_ALLOC_SOURCE src/list-alloc/code.c)
set(LIB_EXPERIMENTAL_SOURCE src/list-experimental/code.c)
set(LIB_PLAYGROUND_SOURCE src/playground/class/v1/class.c)
set(LIB_PLAYGROUND2_SOURCE src/playground/class/v2/class.c)
set(LIB_MEMORY_SOURCE src/playground/memory/v1/memory.c src/playground/memory/v2/memory.c)
set(LIB_BRAIN_SOURCE src/playground/brain/brain.c)
set(LIB_POINTER_SOURCE src/playground/pointer/pointer.c)

set(MAIN_STANDARD_SOURCE tests/main.c ${LIB_STANDARD_SOURCE})
set(MAIN_LIGHT_SOURCE tests/main-light.c ${LIB_LIGHT_SOURCE})
set(MAIN_MICRO_SOURCE tests/main-micro.c ${LIB_MICRO_SOURCE})
set(MAIN_ALLOC_SOURCE tests/main-alloc.c ${LIB_ALLOC_SOURCE})
set(MAIN_EXPERIMENTAL_SOURCE tests/main-experimental.c ${LIB_EXPERIMENTAL_SOURCE})
set(MAIN_PLAYGROUND_SOURCE tests/v1/main-playground.c ${LIB_PLAYGROUND_SOURCE})
set(MAIN_PLAYGROUND2_SOURCE tests/v2/main-playground.c ${LIB_PLAYGROUND2_SOURCE})
set(MAIN_MEMORY_SOURCE tests/main-memory.c ${LIB_MEMORY_SOURCE})
set(MAIN_BRAIN_SOURCE tests/main-brain.c)
set(MAIN_TEST_SOURCE src/playground/test.c)
set(MAIN_ZEN_SOURCE src/playground/zen/zen.c)

include_directories(src)

add_library(list-v1 STATIC ${LIB_LIST1_SOURCE})
add_library(list-v2 STATIC ${LIB_LIST2_SOURCE})
add_library(vm STATIC ${LIB_VM_SOURCE})
add_library(hashtable ${LIB_HASHTABLE_SOURCE})
add_library(list-memory STATIC ${LIB_MEMORY_SOURCE})
add_library(list-common STATIC ${LIB_COMMON_SOURCE})
add_library(list-lcg STATIC ${LIB_LCG_SOURCE})
add_library(list-parameters STATIC ${LIB_PARAMETERS_SOURCE})
add_library(list-standard STATIC ${LIB_STANDARD_SOURCE})
add_library(list-light STATIC ${LIB_LIGHT_SOURCE})
add_library(list-micro STATIC ${LIB_MICRO_SOURCE})
add_library(list-alloc STATIC ${LIB_ALLOC_SOURCE})
add_library(list-experimental STATIC ${LIB_EXPERIMENTAL_SOURCE})
add_library(list-playground STATIC ${LIB_PLAYGROUND_SOURCE})
add_library(list-playground2 STATIC ${LIB_PLAYGROUND2_SOURCE})
add_library(list-brain STATIC ${LIB_BRAIN_SOURCE})
add_library(list-pointer STATIC ${LIB_POINTER_SOURCE})

add_executable(main ${MAIN_STANDARD_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-alloc ${MAIN_ALLOC_SOURCE})
add_executable(main-experimental ${MAIN_EXPERIMENTAL_SOURCE})
add_executable(main-playground ${MAIN_PLAYGROUND_SOURCE})
add_executable(main-playground2 ${MAIN_PLAYGROUND2_SOURCE})
add_executable(main-memory ${MAIN_MEMORY_SOURCE})
add_executable(main-brain ${MAIN_BRAIN_SOURCE})
add_executable(main-test ${MAIN_TEST_SOURCE})

add_executable(zen ${MAIN_ZEN_SOURCE})

target_link_libraries(list-alloc PRIVATE list-parameters)
target_link_libraries(list-pointer PRIVATE list-common)
target_link_libraries(list-experimental PRIVATE list-parameters)

target_link_libraries(main PRIVATE list-v1 list-common)
target_link_libraries(main-light PRIVATE list-light list-common)
target_link_libraries(main-micro PRIVATE list-micro list-common)
target_link_libraries(main-alloc PRIVATE list-alloc list-lcg list-parameters list-common)
target_link_libraries(main-experimental PRIVATE list-experimental list-lcg list-parameters list-common)
target_link_libraries(main-playground PRIVATE list-playground list-micro list-common list-v1)
target_link_libraries(main-playground2 PRIVATE list-playground2 list-micro list-common list-v2)
target_link_libraries(main-memory PRIVATE list-memory list-v1)
target_link_libraries(main-brain PRIVATE list-pointer list-v2 vm hashtable list-micro list-v2)
target_link_libraries(main-test)

configure_file(all_english_words.txt ${CMAKE_BINARY_DIR}/all_english_words.txt COPYONLY)

FILE(WRITE ${CMAKE_BINARY_DIR}/input.txt
    "fopen<\"input.txt\",\"rb\">f (? f==0 perror<\"file not found\" exit<1 : print<\"all ok\")"
)