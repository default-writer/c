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

set(CONFIG_MEMORY_DEBUG_INFO OFF CACHE BOOL "Enable debug information?")
set(CONFIG_MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(CONFIG_MEMORY_ALLOC OFF CACHE BOOL "Enable memory allocator?")

set(CONFIG_MEMORY_DEBUG_INFO OFF)
set(CONFIG_MEMORY_CLEANUP ON)
set(CONFIG_MEMORY_ALLOC OFF)

set(STANDARD
    -save-temps
    -std=gnu99
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
    -Wno-implicit-fallthrough
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
set(LIB_VM1_SOURCE src/playground/vm/v1/vm_v1.c)
set(LIB_VM2_SOURCE src/playground/vm/v2/vm_v2.c)
set(LIB_HASHTABLE1_SOURCE src/playground/hashtable/hashtable.c)
set(LIB_HASHTABLE2_SOURCE src/playground/hashtable/v2/hashtable_v2.c)
set(LIB_STANDARD_SOURCE src/list/code.c)
set(LIB_LIGHT_SOURCE src/list-light/code.c)
set(LIB_MICRO_SOURCE src/list-micro/code.c)
set(LIB_ALLOC_SOURCE src/list-alloc/code.c)
set(LIB_EXPERIMENTAL_SOURCE src/list-experimental/code.c)
set(LIB_PLAYGROUND1_SOURCE src/playground/class/v1/class.c)
set(LIB_PLAYGROUND2_SOURCE src/playground/class/v2/class.c)
set(LIB_MEMORY1_SOURCE
    src/playground/memory/v1/memory_v1.c 
    src/playground/memory/v2/memory_v2.c
    src/playground/memory/v3/memory_v3.c)
set(LIB_MEMORY2_SOURCE 
    src/playground/memory/v4/memory_v4.c)
set(LIB_MEMORY3_SOURCE
    src/playground/memory/v5/memory_v5.c)
set(LIB_MEMORY4_SOURCE
    src/playground/memory/v6/memory_v6.c)
set(LIB_BRAIN_SOURCE src/playground/brain/brain.c)
set(LIB_POINTER1_SOURCE src/playground/pointer/v1/pointer_v1.c)
set(LIB_POINTER2_SOURCE src/playground/pointer/v2/pointer_v2.c)
set(LIB_MEMORY_LIST1_SOURCE src/playground/memory/list/v1/memory_list_v1.c)
set(LIB_MEMORY_LIST2_SOURCE src/playground/memory/list/v2/memory_list_v2.c)
set(LIB_MEMORY_LIST3_SOURCE src/playground/memory/list/v3/memory_list_v3.c)
set(LIB_MEMORY_REF1_SOURCE src/memory/v1/ref_v1.c)
set(LIB_MEMORY_REF2_SOURCE src/memory/v2/ref_v2.c)
set(LIB_MEMORY_REF3_SOURCE src/memory/v3/ref_v3.c)

set(MAIN_STANDARD_SOURCE tests/main.c ${LIB_STANDARD_SOURCE})
set(MAIN_LIGHT_SOURCE tests/main-light.c ${LIB_LIGHT_SOURCE})
set(MAIN_MICRO_SOURCE tests/main-micro.c ${LIB_MICRO_SOURCE})
set(MAIN_ALLOC_SOURCE tests/main-alloc.c ${LIB_ALLOC_SOURCE})
set(MAIN_EXPERIMENTAL_SOURCE tests/main-experimental.c ${LIB_EXPERIMENTAL_SOURCE})
set(MAIN_PLAYGROUND1_SOURCE tests/v1/main-playground_v1.c ${LIB_PLAYGROUND1_SOURCE})
set(MAIN_PLAYGROUND2_SOURCE tests/v2/main-playground_v2.c ${LIB_PLAYGROUND2_SOURCE})
set(MAIN_MEMORY1_SOURCE tests/v1/main-memory_v1.c ${LIB_MEMORY1_SOURCE})
set(MAIN_MEMORY2_SOURCE tests/v2/main-memory_v2.c ${LIB_MEMORY2_SOURCE})
set(MAIN_MEMORY3_SOURCE tests/v3/main-memory_v3.c ${LIB_MEMORY3_SOURCE})
set(MAIN_MEMORY4_SOURCE tests/v4/main-memory_v4.c ${LIB_MEMORY4_SOURCE})
set(MAIN_BRAIN_SOURCE tests/main-brain.c)
set(MAIN_POINTER_SOURCE tests/main-pointer.c)
set(MAIN_HASHTABLE1_SOURCE tests/v1/main-hashtable_v1.c)
set(MAIN_HASHTABLE2_SOURCE tests/v2/main-hashtable_v2.c)
set(MAIN_VM1_SOURCE tests/v1/main-vm1.c)
set(MAIN_VM2_SOURCE tests/v2/main-vm2.c)
set(MAIN_ZEN_SOURCE src/playground/zen/zen.c)
set(TEST_HASHES tests/test-hashes.c)
set(GTK_EXPERIMENT ui/gtk-experiment.c)

include_directories(src)

add_library(list-v1 STATIC ${LIB_LIST1_SOURCE})
add_library(list-v2 STATIC ${LIB_LIST2_SOURCE})
add_library(vm-v1 STATIC ${LIB_VM1_SOURCE})
add_library(vm-v2 STATIC ${LIB_VM2_SOURCE})
add_library(hashtable1 ${LIB_HASHTABLE1_SOURCE})
add_library(hashtable2 ${LIB_HASHTABLE2_SOURCE})
add_library(list-memory1 STATIC ${LIB_MEMORY1_SOURCE})
add_library(list-memory2 STATIC ${LIB_MEMORY2_SOURCE})
add_library(list-memory3 STATIC ${LIB_MEMORY3_SOURCE})
add_library(list-memory4 STATIC ${LIB_MEMORY4_SOURCE})
add_library(list-common STATIC ${LIB_COMMON_SOURCE})
add_library(list-lcg STATIC ${LIB_LCG_SOURCE})
add_library(list-parameters STATIC ${LIB_PARAMETERS_SOURCE})
add_library(list-standard STATIC ${LIB_STANDARD_SOURCE})
add_library(list-light STATIC ${LIB_LIGHT_SOURCE})
add_library(list-micro STATIC ${LIB_MICRO_SOURCE})
add_library(list-alloc STATIC ${LIB_ALLOC_SOURCE})
add_library(list-experimental STATIC ${LIB_EXPERIMENTAL_SOURCE})
add_library(list-playground1 STATIC ${LIB_PLAYGROUND1_SOURCE})
add_library(list-playground2 STATIC ${LIB_PLAYGROUND2_SOURCE})
add_library(list-brain STATIC ${LIB_BRAIN_SOURCE})
add_library(list-pointer1 STATIC ${LIB_POINTER1_SOURCE})
add_library(list-pointer2 STATIC ${LIB_POINTER2_SOURCE})
add_library(memory-list1 STATIC ${LIB_MEMORY_LIST1_SOURCE})
add_library(memory-list2 STATIC ${LIB_MEMORY_LIST2_SOURCE})
add_library(memory-list3 STATIC ${LIB_MEMORY_LIST3_SOURCE})
add_library(memory-ref1 STATIC ${LIB_MEMORY_REF1_SOURCE})
add_library(memory-ref2 STATIC ${LIB_MEMORY_REF2_SOURCE})
add_library(memory-ref3 STATIC ${LIB_MEMORY_REF3_SOURCE})

add_executable(main-default ${MAIN_STANDARD_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-alloc ${MAIN_ALLOC_SOURCE})
add_executable(main-experimental ${MAIN_EXPERIMENTAL_SOURCE})
add_executable(main-playground1 ${MAIN_PLAYGROUND1_SOURCE})
add_executable(main-playground2 ${MAIN_PLAYGROUND2_SOURCE})
add_executable(main-memory1 ${MAIN_MEMORY1_SOURCE})
add_executable(main-memory2 ${MAIN_MEMORY2_SOURCE})
add_executable(main-memory3 ${MAIN_MEMORY3_SOURCE})
add_executable(main-memory4 ${MAIN_MEMORY4_SOURCE})
add_executable(main-brain1 ${MAIN_BRAIN_SOURCE})
add_executable(main-brain2 ${MAIN_BRAIN_SOURCE})
add_executable(main-pointer1 ${MAIN_POINTER_SOURCE})
add_executable(main-pointer2 ${MAIN_POINTER_SOURCE})
add_executable(main-hashtable1 ${MAIN_HASHTABLE1_SOURCE})
add_executable(main-hashtable2 ${MAIN_HASHTABLE2_SOURCE})
add_executable(main-vm1 ${MAIN_VM1_SOURCE})
add_executable(main-vm2 ${MAIN_VM2_SOURCE})
add_executable(main-zen ${MAIN_ZEN_SOURCE})
add_executable(test-hashes ${TEST_HASHES})
add_executable(gtk-experiment ${GTK_EXPERIMENT})

if(TARGET "gtk-experiment")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk4)

    include_directories(${GTK_INCLUDE_DIRS})
    link_directories(${GTK_LIBRARY_DIRS})  
     
    target_link_libraries(gtk-experiment STATIC gtk4)
endif()

target_link_libraries(hashtable1 PRIVATE list-lcg)
target_link_libraries(hashtable2 PRIVATE list-lcg)
target_link_libraries(list-alloc PRIVATE list-parameters)
target_link_libraries(list-pointer1 PRIVATE list-common)
target_link_libraries(list-pointer2 PRIVATE list-common)
target_link_libraries(list-experimental PRIVATE list-parameters)
target_link_libraries(memory-ref1 PRIVATE memory-list1 list-micro list-common)
target_link_libraries(memory-ref2 PRIVATE memory-list2 list-micro list-common)
target_link_libraries(memory-ref3 PRIVATE memory-list3 list-micro list-common)
target_link_libraries(list-pointer2 PRIVATE vm-v2)

target_link_libraries(main-default PRIVATE list-v1 list-common)
target_link_libraries(main-light PRIVATE list-light list-common)
target_link_libraries(main-micro PRIVATE list-micro list-common)
target_link_libraries(main-alloc PRIVATE list-alloc list-lcg list-parameters list-common)
target_link_libraries(main-experimental PRIVATE list-experimental list-lcg list-parameters list-common)
target_link_libraries(main-playground1 PRIVATE list-playground1 list-micro list-common list-v1)
target_link_libraries(main-playground2 PRIVATE list-playground2 list-micro list-common list-v2)
target_link_libraries(main-memory1 PRIVATE list-memory1)
target_link_libraries(main-memory2 PRIVATE list-memory2 memory-ref1)
target_link_libraries(main-memory3 PRIVATE list-memory3 memory-ref2)
target_link_libraries(main-memory4 PRIVATE list-memory4 memory-ref3 list-lcg)
target_link_libraries(main-brain1 PRIVATE list-pointer1 list-micro vm-v1)
target_link_libraries(main-brain2 PRIVATE list-pointer2 list-micro)
target_link_libraries(main-pointer1 PRIVATE list-pointer1 list-micro vm-v1)
target_link_libraries(main-pointer2 PRIVATE list-pointer2 list-micro)
target_link_libraries(main-hashtable1 PRIVATE list-pointer1 list-micro vm-v1 hashtable1)
target_link_libraries(main-hashtable2 PRIVATE list-pointer2 list-micro hashtable2)
target_link_libraries(main-vm1 PRIVATE list-pointer1 list-micro vm-v1)
target_link_libraries(main-vm2 PRIVATE list-pointer2 list-micro)

configure_file(all_english_words.txt ${CMAKE_BINARY_DIR}/all_english_words.txt COPYONLY)

FILE(WRITE ${CMAKE_BINARY_DIR}/input.txt
    "fopen<\"input.txt\",\"rb\">f (? f==0 perror<\"file not found\" exit<1 : print<\"all ok\")"
)

if(TARGETS)

get_property(main_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
get_property(test_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
get_property(gtk_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

list(FILTER main_target_names INCLUDE REGEX "^main-")
list(FILTER test_target_names INCLUDE REGEX "^test-")
list(FILTER gtk_target_names INCLUDE REGEX "^gtk-")

string(REGEX REPLACE ";" " " main_target_names "${main_target_names}")
string(REGEX REPLACE ";" " " test_target_names "${test_target_names}")
string(REGEX REPLACE ";" " " gtk_target_names "${gtk_target_names}")

FILE(WRITE ${CMAKE_BINARY_DIR}/targets.txt
    "${main_target_names} ${test_target_names} ${gtk_target_names}"
)

endif()
