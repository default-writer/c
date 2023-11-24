cmake_minimum_required(VERSION 3.22.1)

project(extensions VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf64")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> ${CMAKE_ASM_NASM_FLAGS} -o <OBJECT> <SOURCE>")

include_directories(../src)

add_subdirectory(.. ../.c)

set(OBERON 
    oberon/method_suite/figures.c
    oberon/method_suite/rectangles.c
    oberon/method_suite/main.c)

set(CLASSES
    oberon/classes/figures.c
    oberon/classes/rectangles.c
    oberon/classes/main.c)

set(SIMPLER_PARSER
    strings/simple_parser.c)

set(TEST_PRIVATE_STRUCTS1_SOURCE 
    structs/private/main-private-structs_v1.c
    structs/lib/v1/object.c)

set(TEST_PRIVATE_STRUCTS2_SOURCE
    structs/private/main-private-structs_v2.c
    structs/lib/v2/object.c
    structs/lib/v2/private.c)

set(TEST_PRIVATE_STRUCTS3_SOURCE
    structs/private/main-private-structs_v3.c
    structs/lib/v3/object.c
    structs/lib/v3/private.c)

set(TEST_PRIVATE_STRUCTS4_SOURCE
    structs/private/main-private-structs_v4.c
    structs/lib/v4/object.c
    structs/lib/v4/private.c)

set(TEST_STRUCTS1_SOURCE
    structs/v1/main-structs_v1.c
    structs/lib/v1/object.c)
set(TEST_STRUCTS2_SOURCE structs/v2/main-structs_v2.c)
set(TEST_STRUCTS3_SOURCE structs/v3/main-structs_v3.c)
set(TEST_STRUCTS3_LIB_SOURCE structs/v3/A.c structs/v3/B.c)
    
set(SUBSTRINGS substrings/main.c)
set(LEETCODE_VALID_PARENTHESES leetcode/valid-parentheses.c)
set(LEETCODE_LONGEST_VALID_PARENTHESES leetcode/longest-valid-parentheses.c)
set(URL_GET url/main.c)
set(GCC_SOURCE gcc/src/main.c)
set(GCC_NASM_SOURCE gcc/src/my_asm_program.asm)

set_source_files_properties(${GCC_NASM_SOURCE} PROPERTIES LANGUAGE ASM_NASM)

add_library(demo-gcc-nasm OBJECT ${GCC_NASM_SOURCE})
add_library(structs-v3 STATIC ${TEST_STRUCTS3_LIB_SOURCE})

add_executable(c-oberon-method-suite ${OBERON})
add_executable(c-classes ${CLASSES})
add_executable(c-simple-parser ${SIMPLER_PARSER})
add_executable(c-substrings ${SUBSTRINGS})
add_executable(c-valid-parentheses ${LEETCODE_VALID_PARENTHESES})
add_executable(c-longest-valid-parentheses ${LEETCODE_LONGEST_VALID_PARENTHESES})
add_executable(demo-url ${URL_GET})
add_executable(demo-gcc ${GCC_SOURCE} $<TARGET_OBJECTS:demo-gcc-nasm>)
add_executable(main-private-structs1 ${TEST_PRIVATE_STRUCTS1_SOURCE})
add_executable(main-private-structs2 ${TEST_PRIVATE_STRUCTS2_SOURCE})
add_executable(main-private-structs3 ${TEST_PRIVATE_STRUCTS3_SOURCE})
add_executable(main-private-structs4 ${TEST_PRIVATE_STRUCTS4_SOURCE})
add_executable(main-structs1 ${TEST_STRUCTS1_SOURCE})
add_executable(main-structs2 ${TEST_STRUCTS2_SOURCE})
add_executable(main-structs3 ${TEST_STRUCTS3_SOURCE})

set_target_properties(demo-gcc PROPERTIES NASM_OBJ_FORMAT elf64)

target_link_libraries(c-substrings PRIVATE ctypes std-os1)
target_link_libraries(demo-url PRIVATE curl ctypes)
target_link_libraries(main-private-structs1 PRIVATE list-common)
target_link_libraries(main-private-structs2 PRIVATE list-common)
target_link_libraries(main-private-structs3 PRIVATE list-common)
target_link_libraries(main-private-structs4 PRIVATE list-common)
target_link_libraries(main-structs1 PRIVATE list-common)
target_link_libraries(main-structs2 PRIVATE list-class)
target_link_libraries(main-structs3 PRIVATE structs-v3)
target_link_libraries(structs-v3 PRIVATE list-class)
