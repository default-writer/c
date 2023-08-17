cmake_minimum_required(VERSION 3.22.1)

project(c VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CONFIG_MEMORY_DEBUG_INFO "Enables memory debug information")
option(CONFIG_MEMORY_CLEANUP "Enables memory cleanup")
option(CONFIG_MEMORY_ALLOC "Enables memory allocator")
option(CONFIG_MEMCPY "Enables use of memcpy")

set(CONFIG_MEMORY_DEBUG_INFO ON CACHE BOOL "Enable debug information?")
set(CONFIG_MEMORY_CLEANUP ON CACHE BOOL "Enable memory cleanup?")
set(CONFIG_MEMORY_ALLOC OFF CACHE BOOL "Enable memory allocator?")
set(CONFIG_MEMCPY OFF CACHE BOOL "Enable use of memcpy?")

set(CONFIG_MEMORY_DEBUG_INFO ON)
set(CONFIG_MEMORY_CLEANUP ON)
set(CONFIG_MEMORY_ALLOC OFF)
set(CONFIG_MEMCPY OFF)

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
        # -Wpedantic
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

if(CONFIG_MEMORY_DEBUG_INFO)
    if(VERBOSE)
        message(STATUS "configure with memory debug information")
    endif()
    add_compile_definitions(CONFIG_MEMORY_DEBUG_INFO)
    add_definitions(-DCONFIG_MEMORY_DEBUG_INFO)
endif()

if(UNIX)
    add_definitions(-D_POSIX_C_SOURCE=199309L)
endif()

set(SUBSTRINGS substrings/main.c)

include_directories( $ENV{C_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}../src )
link_directories( $ENV{C_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}../lib )

add_executable(c-substrings ${SUBSTRINGS})

target_link_libraries(c-substrings PRIVATE ctypes)
