cmake_minimum_required(VERSION 3.22.1)

project(c VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CONFIG_VM_DEBUG_INFO "Enables memory vm debug information")
option(CONFIG_MEMORY_DEBUG_INFO "Enables memory debug information")
option(CONFIG_MEMORY_CLEANUP "Enables memory cleanup")
option(CONFIG_MEMORY_ALLOC "Enables memory allocator")
option(CONFIG_MEMCPY "Enables use of memcpy")

set(CONFIG_VM_DEBUG_INFO ON CACHE BOOL "Enable debug information?")
set(CONFIG_MEMORY_DEBUG_INFO ON CACHE BOOL "Enable debug information?")
set(CONFIG_MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(CONFIG_MEMORY_ALLOC OFF CACHE BOOL "Enable memory allocator?")
set(CONFIG_MEMCPY OFF CACHE BOOL "Enable use of memcpy?")

set(CONFIG_VM_DEBUG_INFO ON)
set(CONFIG_MEMORY_DEBUG_INFO ON)
set(CONFIG_MEMORY_CLEANUP ON)
set(CONFIG_MEMORY_ALLOC OFF)
set(CONFIG_MEMCPY OFF)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

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
        -std=gnu89
        -fgnu89-inline
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
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows specific configuration
    message("Detected Windows OS")
    # You can add specific CMake options or behavior for Windows here
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS specific configuration
    message("Detected macOS OS")
    # You can add specific CMake options or behavior for macOS here
else()
    # Other/Unknown OS
    message("Unknown OS")
    # You can add behavior for other/unknown OS here if needed
endif()

set(STRICT
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wold-style-definition
)

if(CONFIG_VM_DEBUG_INFO)
    if(VERBOSE)
        message(STATUS "configure with vm memory debug information")
    endif()
    add_compile_definitions(CONFIG_VM_DEBUG_INFO)
    add_definitions(-DCONFIG_VM_DEBUG_INFO)
endif()

if(CONFIG_MEMORY_DEBUG_INFO)
    if(VERBOSE)
        message(STATUS "configure with memory debug information")
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

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -ftest-coverage")
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

set(LIB_MEMORY_SOURCE src/generic/memory_v1.c)
set(LIB_LIST_SOURCE src/list/list_v1.c)

set(LIB_STD_TYPES1_SOURCE
    src/vm/v1/options/options_v1.c
    src/vm/v1/os/os_v1.c
    src/vm/v1/pointer/pointer_v1.c
    src/vm/v1/types_v1.c
    src/vm/v1/system/info_v1.c
    src/vm/v1/types/data/data_v1.c
    src/vm/v1/types/file/file_v1.c
    src/vm/v1/types/list/list_v1.c
    src/vm/v1/types/object/object_v1.c
    src/vm/v1/types/string/string_v1.c
    src/vm/v1/types/string_pointer/string_pointer_v1.c
    src/vm/v1/types/user/user_v1.c
    src/vm/v1/virtual/virtual_v1.c)

set(TESTS_SRC_LIST tests/vm/v1/test_list.c)
set(TESTS_SRC_POINTER tests/vm/v1/test_pointer.c)
set(TESTS_SRC_VM_V1 tests/vm/v1/test_vm_v1.c)
set(TESTS_SRC_VM_V1 tests/vm/v1/test_vm_v1.c)
# set(MAIN_TESTS_VM1_SOURCE tests/vm/v1/main-tests_v1.c)

include_directories(src)

add_library(cmemory SHARED ${LIB_MEMORY_SOURCE})
add_library(clist SHARED ${LIB_LIST_SOURCE})
add_library(ctypes SHARED ${LIB_STD_TYPES1_SOURCE})

# test
add_library(tests-vm-v1 STATIC 
    ${TESTS_SRC_LIST}
    ${TESTS_SRC_POINTER}
    ${TESTS_SRC_VM_V1})

# main 
# add_executable(main-tests-vm1 ${MAIN_TESTS_VM1_SOURCE})

# test
target_link_libraries(tests-vm-v1 PRIVATE ctypes clist cmemory)
target_link_libraries(clist PRIVATE cmemory)
target_link_libraries(ctypes PRIVATE clist cmemory)

# main
# target_link_libraries(main-tests-vm1 PRIVATE cmemory clist ctypes tests-vm-v1)

configure_file(all_english_words.txt ${CMAKE_BINARY_DIR}/all_english_words.txt COPYONLY)
configure_file(file.txt ${CMAKE_BINARY_DIR}/file.txt COPYONLY)

FILE(WRITE ${CMAKE_BINARY_DIR}/input.txt
    "fopen<\"input.txt\",\"rb\">f (? f==0 perror<\"file not found\" exit<1 : print<\"all ok\")"
)

FILE(WRITE ${CMAKE_BINARY_DIR}/input-substrings.txt
    "juadassjuuiilajuasasjuas\n"
    "ju\n"
    "juadassjuuiilajuasasjuas\n"
    "as\n"
)

execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE GIT_COMMIT_HASH)
execute_process(COMMAND git log -n 1 --pretty=format:%at OUTPUT_VARIABLE UNIX_TIMESTAMP)
string(REGEX REPLACE "\n" "" GIT_COMMIT_HASH "${GIT_COMMIT_HASH}")
string(REGEX REPLACE "\n" "" UNIX_TIMESTAMP "${UNIX_TIMESTAMP}")
file(WRITE ${CMAKE_SOURCE_DIR}/src/version.h 
    "#define GIT_COMMIT_HASH \"" ${GIT_COMMIT_HASH} "\"\n"
    "#define UNIX_TIMESTAMP " ${UNIX_TIMESTAMP} "\n"
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
