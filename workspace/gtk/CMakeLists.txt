cmake_minimum_required(VERSION 3.22.1)

project(gtk VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_directories( $ENV{LD_LIBRARY_PATH} )

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

if(UNIX)
    add_definitions(-D_POSIX_C_SOURCE=199309L)
endif()

# gtk
set(GTK_EXPERIMENT examples/gtk-experiment1.c)

include_directories(${CMAKE_SOURCE_DIR})

# gtk
add_executable(gtk-experiment1 ${GTK_EXPERIMENT})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
target_link_libraries(gtk-experiment1 PkgConfig::GTK4)

target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/gtk-4.0)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/glib-2.0)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/gio-unix-2.0)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/cairo)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/pango-1.0)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/harfbuzz)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/fribidi)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/gdk-pixbuf-2.0)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/pixman-1)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/uuid)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/freetype2)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/libpng16)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/graphene-1.0)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/libmount)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/include/blkid)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/lib/x86_64-linux-gnu/glib-2.0/include)
target_include_directories(gtk-experiment1 SYSTEM PUBLIC /usr/lib/x86_64-linux-gnu/graphene-1.0/include)
