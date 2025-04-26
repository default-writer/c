cmake_minimum_required(VERSION 3.22.1)

project(c VERSION 0.0.1 LANGUAGES C CXX)
include(GenerateExportHeader)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CONFIG_MEMORY_DEBUG_INFO "Enables memory vm debug information")
option(CONFIG_MEMORY_CLEANUP "Enables memory cleanup")
option(CONFIG_MEMORY_ALLOC "Enables memory allocator")
option(CONFIG_MEMCPY "Enables use of memcpy")
option(CONFIG_TTY "Enable tty output?")

set(CONFIG_MEMORY_DEBUG_INFO ON CACHE BOOL "Enable debug information?")
set(CONFIG_MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(CONFIG_MEMORY_ALLOC OFF CACHE BOOL "Enable memory allocator?")
set(CONFIG_MEMCPY OFF CACHE BOOL "Enable use of memcpy?")
set(CONFIG_TTY OFF CACHE BOOL "Enable tty output?")

execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE GIT_COMMIT_HASH)
execute_process(COMMAND git log -n 1 --pretty=format:%at OUTPUT_VARIABLE UNIX_TIMESTAMP)
string(REGEX REPLACE "\n" "" GIT_COMMIT_HASH "${GIT_COMMIT_HASH}")
string(REGEX REPLACE "\n" "" UNIX_TIMESTAMP "${UNIX_TIMESTAMP}")
file(WRITE ${CMAKE_SOURCE_DIR}/src/std/version.h 
    "#define MAJOR_VERSION " ${CMAKE_PROJECT_VERSION_MAJOR} "\n"
    "#define MINOR_VERSION " ${CMAKE_PROJECT_VERSION_MINOR} "\n"
    "#define PATCH_VERSION " ${CMAKE_PROJECT_VERSION_PATCH} "\n"
    "#define GIT_COMMIT_HASH_VALUE " ${GIT_COMMIT_HASH} "\n"
    "#define GIT_COMMIT_HASH \"" ${GIT_COMMIT_HASH} "\"\n"
    "#define UNIX_TIMESTAMP " ${UNIX_TIMESTAMP} "\n"
)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# Set output directories for all platforms
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# For Windows-specific DLL placement
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# Check the operating system version
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    set(INLINE 
        -finline-functions-called-once
    )

    set(CLANGD
        -fsanitize=bounds-strict
        -static-libasan
    )

    set(STANDARD
        -save-temps
        -std=gnu99
        -fgnu89-inline
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(SANITIZE
            -fsanitize=address,leak,pointer-compare,pointer-subtract,float-cast-overflow,float-divide-by-zero
            -fsanitize-undefined-trap-on-error
            -fno-sanitize-recover=all
        )
    else()
        set(SANITIZE
            -fsanitize=address,undefined,leak,pointer-compare,pointer-subtract,float-cast-overflow,float-divide-by-zero
            -fsanitize-undefined-trap-on-error
            -fno-sanitize-recover=all
        )
    endif()

    set(NO_SANITIZE
        -fno-sanitize=all
    )

    set(OPTIONS
        -Wpedantic
        -Winline
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
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-clobbered
    )
endif()

set(STRICT
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wold-style-definition
)

if(CONFIG_TTY)
    if(VERBOSE)
        message(STATUS "configure with TTY ASCII ESC output information")
    endif()
    add_compile_definitions(CONFIG_TTY)
    add_definitions(-DCONFIG_TTY)
endif()

if(CONFIG_MEMORY_DEBUG_INFO)
    if(VERBOSE)
        message(STATUS "configure with vm memory debug information")
    endif()
    add_compile_definitions(CONFIG_MEMORY_DEBUG_INFO)
    add_definitions(-DCONFIG_MEMORY_DEBUG_INFO)
endif()

if(CONFIG_MEMORY_CLEANUP)
    if(VERBOSE)
        message(STATUS "configure with memory cleanup")
    endif()
    add_compile_definitions(CONFIG_MEMORY_CLEANUP)
    add_definitions(-DCONFIG_MEMORY_CLEANUP)
endif()

if(CONFIG_MEMORY_ALLOC)
    if(VERBOSE)
        message(STATUS "configure with memory allocator")
    endif()
    add_compile_definitions(CONFIG_MEMORY_ALLOC)
    add_definitions(-DCONFIG_MEMORY_ALLOC)
endif()

if(CONFIG_MEMCPY)
    if(VERBOSE)
        message(STATUS "configure with use of memcpy")
    endif()
    add_compile_definitions(CONFIG_MEMCPY)
    add_definitions(-DCONFIG_MEMCPY)
endif()

if(CODE_SANITIZER)
    if(VERBOSE)
        message("building with sanitizers")
    endif()
    add_compile_options(${STANDARD} ${SANITIZE} ${OPTIONS})
    add_link_options(${STANDARD} ${SANITIZE} ${OPTIONS})
else()
    if(VERBOSE)
        message("building without sanitizers")
    endif()
    add_compile_options(${STANDARD} ${NO_SANITIZE})
    add_link_options(${STANDARD} ${NO_SANITIZE})
endif()

if(MOCKS)
    if(VERBOSE)
        message("building with mocks")
    endif()
    add_compile_definitions(MOCKS)
    add_definitions(-DMOCKS)
else()
    if(VERBOSE)
        message("building without mocks")
    endif()
endif()

if(CONFIG_GC)
    if(VERBOSE)
        message("building with garbage collector")
    endif()
    add_compile_definitions(CONFIG_GC)
    add_definitions(-DCONFIG_GC)
else()
    if(VERBOSE)
        message("building without garbage collector")
    endif()
endif()

if(CODE_COVERAGE)
    if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        if(VERBOSE)
            message("building with llvm code coverage tools")
        endif()
        # Warning/Error messages
        if(NOT LLVM_COV_PATH)
            if(VERBOSE)
                message(FATAL_ERROR "llvm-cov not found! Aborting.")
            endif()
        endif()

        # set Flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

    elseif(CMAKE_COMPILER_IS_GNUCXX)
        if(VERBOSE)
            message("building with lcov code coverage tools")
        endif()

        # Warning/Error messages
        if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
            if(VERBOSE)
                message(WARNING "code coverage results with an optimized (non-Debug) build may be misleading")
            endif()
        endif()
        if(NOT LCOV_PATH)
            if(VERBOSE)
                message(FATAL_ERROR "lcov not found! Aborting...")
            endif()
        endif()
        if(NOT GENHTML_PATH)
            if(VERBOSE)
                message(FATAL_ERROR "genhtml not found! Aborting...")
            endif()
        endif()

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    else()
        if(VERBOSE)
            message(FATAL_ERROR "code coverage requires Clang or GCC. Aborting.")
        endif()
    endif()
    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang" OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-ftest-coverage)
        link_libraries(gcov)
    endif()
endif()

if(UNIX)
    add_definitions(-D_POSIX_C_SOURCE=199309L)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s")
endif()

set(LIB_SYS_SOURCE
    src/system/api/api_v1.c
    src/system/error/error_v1.c
    src/system/hashtable/hashtable_v1.c
    src/system/info/info_v1.c
    src/system/memory/memory_v1.c
    src/system/options/options_v1.c
    src/system/os/os_v1.c
)

set(LIB_VM_SOURCE
    src/virtual/api/api_v1.c
    src/virtual/pointer/pointer_v1.c
    src/virtual/types/data/data_v1.c
    src/virtual/types/file/file_v1.c
    src/virtual/types/object/object_v1.c
    src/virtual/types/stack/stack_v1.c
    src/virtual/types/string/string_v1.c
    src/virtual/types/user/user_v1.c
    src/virtual/virtual/virtual_v1.c
    src/virtual/vm/vm_v1.c
    src/virtual/env/env_v1.c
    src/virtual/list/list_v1.c
)

set(TESTS_SOURCE
    tests/vm/test_list.c
    tests/vm/test_memory.c
    tests/vm/test_pointer.c
    tests/vm/test_vm.c
    tests/vm/test_hashtable.c
    tests/vm/test_api.c
)

set(MAIN_TESTS_VM1_SOURCE tests/vm/main-tests_v1.c)

include_directories(/usr/include/python3.12)
include_directories(${CMAKE_SOURCE_DIR}/.deps/rexo/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests)

add_library(c-sys SHARED ${LIB_SYS_SOURCE})
generate_export_header(c-sys
    BASE_NAME csys
    EXPORT_MACRO_NAME CSYS_EXPORT
    EXPORT_FILE_NAME ${CMAKE_SOURCE_DIR}/src/system/export.h
)
target_include_directories(c-sys PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/lib
)

add_library(c-vm SHARED ${LIB_VM_SOURCE})
generate_export_header(c-vm
    BASE_NAME cvm
    EXPORT_MACRO_NAME CVM_EXPORT
    EXPORT_FILE_NAME ${CMAKE_SOURCE_DIR}/src/virtual/export.h
)
target_include_directories(c-vm PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/lib
)


# main 
add_executable(main-tests-vm1
    ${MAIN_TESTS_VM1_SOURCE}
    ${TESTS_SOURCE}
)

# test
target_link_libraries(c-vm PUBLIC
    c-sys
)

# main
target_link_libraries(main-tests-vm1 PUBLIC
    c-sys
    c-vm
)
target_link_directories(main-tests-vm1 PUBLIC ${CMAKE_SOURCE_DIR}/lib)

configure_file(data/all_english_words.txt ${CMAKE_BINARY_DIR}/data/all_english_words.txt COPYONLY)
configure_file(data/file.txt ${CMAKE_BINARY_DIR}/data/file.txt COPYONLY)

FILE(WRITE ${CMAKE_BINARY_DIR}/data/input.txt
    "fopen<\"input.txt\",\"rb\">f (? f==0 perror<\"file not found\" exit<1 : print<\"all ok\")"
)

FILE(WRITE ${CMAKE_BINARY_DIR}/input-substrings.txt
    "juadassjuuiilajuasasjuas\n"
    "ju\n"
    "juadassjuuiilajuasasjuas\n"
    "as\n"
)

if(TARGETS)
    get_property(main_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

    list(FILTER main_target_names INCLUDE REGEX "^main-")

    string(REGEX REPLACE ";" " " main_target_names "${main_target_names}")

    file(WRITE ${CMAKE_BINARY_DIR}/targets.txt
        "${main_target_names}"
    )

    get_property(target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target_name ${target_names})
        get_target_property(TARGET_SOURCES ${target_name} SOURCES)
        set(output_dir "${CMAKE_BINARY_DIR}/config-${target_name}")
        file(MAKE_DIRECTORY ${output_dir})

        file(WRITE ${output_dir}/sources.txt "")
        foreach(source_file ${TARGET_SOURCES})
            file(APPEND ${output_dir}/sources.txt "${source_file}\n")
        endforeach()
    endforeach()

    file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.h" "examples/*.c" "examples/*.h")
    file(WRITE ${CMAKE_BINARY_DIR}/all-sources.txt "")
    foreach(source_file ${SOURCE_FILES})
        file(APPEND ${CMAKE_BINARY_DIR}/all-sources.txt "${source_file}\n")
    endforeach()
endif()
