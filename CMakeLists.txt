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

link_directories( $ENV{LD_LIBRARY_PATH} )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $ENV{LD_LIBRARY_PATH})

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

if(GC)
    if(VERBOSE)
        message("building with garbage collector")
    endif()
    add_compile_definitions(GC)
    add_definitions(-DGC)
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

set(LIB_COMMON_SOURCE src/common/alloc.c)
set(LIB_LCG_SOURCE src/common/lcg.c)
set(LIB_PARAMETERS_SOURCE src/common/parameters.c)

set(LIB_LIST1_SOURCE src/playground/list/v1/list_v1.c)
set(LIB_LIST2_SOURCE src/playground/list/v2/list_v2.c)
set(LIB_VM1_SOURCE src/vm/v1/vm_v1.c)
set(LIB_VM2_SOURCE src/vm/v2/vm_v2.c)
set(LIB_HASHTABLE1_SOURCE src/playground/hashtable/v1/hashtable_v1.c)
set(LIB_HASHTABLE2_SOURCE src/playground/hashtable/v2/hashtable_v2.c)
set(LIB_STANDARD_SOURCE src/list/v1/list_v1.c)
set(LIB_LIGHT_SOURCE src/list-light/code.c)
set(LIB_MICRO_SOURCE src/list-micro/code.c)
set(LIB_ALLOC_SOURCE src/list-alloc/code.c)
set(LIB_EXPERIMENTAL_SOURCE src/list-experimental/code.c)
set(LIB_CLASS1_SOURCE src/playground/class/v1/class_v1.c)
set(LIB_CLASS2_SOURCE src/playground/class/v2/class_v2.c)
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
set(LIB_POINTER1_SOURCE src/pointer/v1/pointer_v1.c)
set(LIB_POINTER2_SOURCE src/pointer/v2/pointer_v2.c)
set(LIB_MEMORY_LIST1_SOURCE src/playground/memory/list/v1/memory_list_v1.c)
set(LIB_MEMORY_LIST2_SOURCE src/playground/memory/list/v2/memory_list_v2.c)
set(LIB_MEMORY_LIST3_SOURCE src/playground/memory/list/v3/memory_list_v3.c)
set(LIB_MEMORY_REF1_SOURCE src/memory/v1/ref_v1.c)
set(LIB_MEMORY_REF2_SOURCE src/memory/v2/ref_v2.c)
set(LIB_MEMORY_REF3_SOURCE src/memory/v3/ref_v3.c)
set(LIB_CLASS_SOURCE src/class/class.c)
set(LIB_STD_DATA1_SOURCE src/pointer/types/data/v1/data_v1.c)
set(LIB_STD_USER1_SOURCE src/pointer/types/user/v1/user_v1.c)
set(LIB_STD_LIST1_SOURCE src/pointer/types/list/v1/list_v1.c)
set(LIB_STD_FILE1_SOURCE src/pointer/types/file/v1/file_v1.c)
set(LIB_STD_STRING1_SOURCE src/pointer/types/string/v1/string_v1.c)
set(LIB_STD_STRING_POINTER1_SOURCE src/pointer/types/string_pointer/v1/string_pointer_v1.c)
set(LIB_STD_OBJECT1_SOURCE src/pointer/types/object/v1/object_v1.c)
set(LIB_STD_OS1_SOURCE src/pointer/types/os/v1/os_v1.c)
set(LIB_STD_TYPES1_SOURCE
    src/pointer/v1/pointer_v1.c
    src/pointer/types/void/v1/void_v1.c
    src/pointer/types/data/v1/data_v1.c
    src/pointer/types/user/v1/user_v1.c
    src/pointer/types/string/v1/string_v1.c
    src/pointer/types/string_pointer/v1/string_pointer_v1.c
    src/pointer/types/file/v1/file_v1.c
    src/pointer/types/list/v1/list_v1.c
    src/pointer/types/object/v1/object_v1.c
    src/pointer/types/os/v1/os_v1.c)

set(LIB_LINKED_LIST_SOURCE
    src/stl/linked_list.c
    src/stl/linked_list_enumerator.c
    src/stl/linked_list_static_enumerator.c)

# test
set(TEST_HASHES_SOURCE tests/test-hashes.c)
set(TEST_LINKED_LIST_SOURCE tests/main-linked-list.c)
set(TEST_LINKED_LIST_STATIC_ENUMERATOR_SOURCE tests/main-linked-list-static-enumerator.c)
set(TEST_STRUCTS1_SOURCE tests/structs/v1/main-structs_v1.c)
set(TEST_STRUCTS2_SOURCE tests/structs/v2/main-structs_v2.c)
set(TEST_STRUCTS3_SOURCE tests/structs/v3/main-structs_v3.c)
set(TEST_STRUCTS3_LIB_SOURCE tests/structs/v3/A.c tests/structs/v3/B.c)

set(TESTS_SRC_LIST tests/src/test_list/test_list.c)
set(TESTS_SRC_LIST_MICRO tests/src/test_list-micro/test_list-micro.c)
set(TESTS_SRC_LIST_ALLOC tests/src/test_list-alloc/test_list-alloc.c)
set(TESTS_SRC_LIST_EXPERIMENTAL tests/src/test_list-experimental/test_list-experimental.c)
set(TESTS_SRC_VM_V1 tests/src/vm/v1/vm_v1.c)
set(TESTS_SRC_VM_V2 tests/src/vm/v2/vm_v2.c)

# main
set(MAIN_STANDARD_SOURCE tests/main-default.c)
set(MAIN_LIGHT_SOURCE tests/main-light.c ${LIB_LIGHT_SOURCE})
set(MAIN_MICRO_SOURCE tests/main-micro.c)
set(MAIN_ALLOC_SOURCE tests/main-alloc.c)
set(MAIN_EXPERIMENTAL_SOURCE tests/main-experimental.c)
set(MAIN_PLAYGROUND1_SOURCE tests/v1/main-playground_v1.c ${LIB_CLASS1_SOURCE})
set(MAIN_PLAYGROUND2_SOURCE tests/v2/main-playground_v2.c ${LIB_CLASS2_SOURCE})
set(MAIN_MEMORY1_SOURCE tests/v1/main-memory_v1.c ${LIB_MEMORY1_SOURCE})
set(MAIN_MEMORY2_SOURCE tests/v2/main-memory_v2.c ${LIB_MEMORY2_SOURCE})
set(MAIN_MEMORY3_SOURCE tests/v3/main-memory_v3.c ${LIB_MEMORY3_SOURCE})
set(MAIN_MEMORY4_SOURCE tests/v4/main-memory_v4.c ${LIB_MEMORY4_SOURCE})
set(MAIN_BRAIN1_SOURCE tests/v1/main-brain_v1.c)
set(MAIN_BRAIN2_SOURCE tests/v2/main-brain_v2.c)
set(MAIN_POINTER1_SOURCE tests/v1/main-pointer_v1.c)
set(MAIN_POINTER2_SOURCE tests/v2/main-pointer_v2.c)
set(MAIN_HASHTABLE1_SOURCE tests/v1/main-hashtable_v1.c)
set(MAIN_HASHTABLE2_SOURCE tests/v2/main-hashtable_v2.c)
set(MAIN_VM1_SOURCE tests/v1/main-vm1.c)
set(MAIN_VM2_SOURCE tests/v2/main-vm2.c)
set(MAIN_ZEN_SOURCE src/playground/zen/zen.c)
set(MAIN_TESTS_VM1_SOURCE tests/v1/main-tests_v1.c)
set(MAIN_TESTS_VM2_SOURCE tests/v2/main-tests_v2.c)

include_directories(src)

# test
add_library(tests STATIC
    ${TESTS_SRC_LIST}
    ${TESTS_SRC_LIST_ALLOC}
    ${TESTS_SRC_LIST_EXPERIMENTAL}
    ${TESTS_SRC_LIST_MICRO}
)

add_library(tests-vm-v1 STATIC ${TESTS_SRC_VM_V1})
add_library(tests-vm-v2 STATIC ${TESTS_SRC_VM_V2})

#user
add_library(user1 SHARED ${LIB_STD_USER1_SOURCE})

# all
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
add_library(list-default STATIC ${LIB_STANDARD_SOURCE})
add_library(list-light STATIC ${LIB_LIGHT_SOURCE})
add_library(list-micro STATIC ${LIB_MICRO_SOURCE})
add_library(list-alloc STATIC ${LIB_ALLOC_SOURCE})
add_library(list-experimental STATIC ${LIB_EXPERIMENTAL_SOURCE})
add_library(list-class1 STATIC ${LIB_CLASS1_SOURCE})
add_library(list-class2 STATIC ${LIB_CLASS2_SOURCE})
add_library(list-class STATIC ${LIB_CLASS_SOURCE})
add_library(list-brain STATIC ${LIB_BRAIN_SOURCE})
add_library(list-pointer1 STATIC ${LIB_POINTER1_SOURCE})
add_library(list-pointer2 STATIC ${LIB_POINTER2_SOURCE})
add_library(ctypes SHARED
    ${LIB_COMMON_SOURCE}
    ${LIB_MICRO_SOURCE}
    ${LIB_VM1_SOURCE}
    ${LIB_STD_TYPES1_SOURCE}
    ${LIB_POINTER1_SOURCE})
add_library(memory-list1 STATIC ${LIB_MEMORY_LIST1_SOURCE})
add_library(memory-list2 STATIC ${LIB_MEMORY_LIST2_SOURCE})
add_library(memory-list3 STATIC ${LIB_MEMORY_LIST3_SOURCE})
add_library(memory-ref1 STATIC ${LIB_MEMORY_REF1_SOURCE})
add_library(memory-ref2 STATIC ${LIB_MEMORY_REF2_SOURCE})
add_library(memory-ref3 STATIC ${LIB_MEMORY_REF3_SOURCE})
add_library(structs-v3 STATIC ${TEST_STRUCTS3_LIB_SOURCE})
add_library(stl-linked-list STATIC ${LIB_LINKED_LIST_SOURCE})
add_library(std-types1 STATIC ${LIB_STD_TYPES1_SOURCE})
add_library(std-list1 SHARED ${LIB_STD_LIST1_SOURCE})
add_library(std-file1 SHARED ${LIB_STD_FILE1_SOURCE})
add_library(std-string1 SHARED ${LIB_STD_STRING1_SOURCE})
add_library(std-string-pointer1 SHARED ${LIB_STD_STRING_POINTER1_SOURCE})
add_library(std-data1 SHARED ${LIB_STD_DATA1_SOURCE})
add_library(std-os1 SHARED ${LIB_STD_OS1_SOURCE})
add_library(std-object1 SHARED ${LIB_STD_OBJECT1_SOURCE})

# test
add_executable(test-hashes ${TEST_HASHES_SOURCE})

# main 
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
add_executable(main-brain1 ${MAIN_BRAIN1_SOURCE})
add_executable(main-brain2 ${MAIN_BRAIN2_SOURCE})
add_executable(main-pointer1 ${MAIN_POINTER1_SOURCE})
add_executable(main-pointer2 ${MAIN_POINTER2_SOURCE})
add_executable(main-hashtable1 ${MAIN_HASHTABLE1_SOURCE})
add_executable(main-hashtable2 ${MAIN_HASHTABLE2_SOURCE})
add_executable(main-vm1 ${MAIN_VM1_SOURCE})
add_executable(main-vm2 ${MAIN_VM2_SOURCE})
add_executable(main-zen ${MAIN_ZEN_SOURCE})
add_executable(main-linked-list ${TEST_LINKED_LIST_SOURCE})
add_executable(main-linked-list-static-enumerator ${TEST_LINKED_LIST_STATIC_ENUMERATOR_SOURCE})
add_executable(main-structs1 ${TEST_STRUCTS1_SOURCE})
add_executable(main-structs2 ${TEST_STRUCTS2_SOURCE})
add_executable(main-structs3 ${TEST_STRUCTS3_SOURCE})
add_executable(main-tests-vm1 ${MAIN_TESTS_VM1_SOURCE})
add_executable(main-tests-vm2 ${MAIN_TESTS_VM2_SOURCE})

# examples
add_executable(c-substrings examples/substrings/main.c)

# all
target_link_libraries(hashtable1 PRIVATE list-lcg)
target_link_libraries(hashtable2 PRIVATE list-lcg)
target_link_libraries(list-memory1 PRIVATE list-common)
target_link_libraries(list-memory2 PRIVATE list-common)
target_link_libraries(list-memory3 PRIVATE list-common)
target_link_libraries(list-memory4 PRIVATE list-common)
target_link_libraries(list-class PRIVATE list-common)
target_link_libraries(list-alloc PRIVATE list-parameters)
target_link_libraries(list-pointer1 PRIVATE list-common list-micro std-types1)
target_link_libraries(list-pointer2 PRIVATE list-common)
target_link_libraries(list-experimental PRIVATE list-parameters)
target_link_libraries(memory-ref1 PRIVATE memory-list1 list-micro list-common)
target_link_libraries(memory-ref2 PRIVATE memory-list2 list-micro list-common)
target_link_libraries(memory-ref3 PRIVATE memory-list3 list-micro list-common)
target_link_libraries(list-pointer2 PRIVATE vm-v2)
target_link_libraries(structs-v3 PRIVATE list-class)
target_link_libraries(stl-linked-list PRIVATE list-common)
target_link_libraries(c-substrings PRIVATE ctypes)

# test
target_link_libraries(tests PRIVATE list-alloc list-parameters)
target_link_libraries(tests-vm-v1 PRIVATE tests list-common list-micro list-pointer1)
target_link_libraries(tests-vm-v2 PRIVATE tests list-common list-micro list-pointer2)

# main
target_link_libraries(main-default PRIVATE tests list-common list-alloc list-v1 list-default)
target_link_libraries(main-light PRIVATE tests list-common list-alloc  list-micro list-light)
target_link_libraries(main-micro PRIVATE tests list-common list-alloc list-micro)
target_link_libraries(main-alloc PRIVATE tests list-common list-alloc list-lcg list-parameters)
target_link_libraries(main-experimental PRIVATE tests list-common list-experimental list-lcg list-parameters)
target_link_libraries(main-playground1 PRIVATE list-default list-class1 list-micro list-common list-v1)
target_link_libraries(main-playground2 PRIVATE list-class2 list-micro list-common list-v2)
target_link_libraries(main-memory1 PRIVATE list-memory1)
target_link_libraries(main-memory2 PRIVATE list-memory2 memory-ref1)
target_link_libraries(main-memory3 PRIVATE list-memory3 memory-ref2)
target_link_libraries(main-memory4 PRIVATE list-memory4 memory-ref3 list-lcg)
target_link_libraries(main-brain1 PRIVATE tests list-default tests-vm-v1 list-pointer1 list-micro vm-v1 std-string-pointer1)
target_link_libraries(main-brain2 PRIVATE list-pointer2 list-micro)
target_link_libraries(main-pointer1 PRIVATE list-pointer1 list-micro vm-v1)
target_link_libraries(main-pointer2 PRIVATE list-pointer2 list-micro)
target_link_libraries(main-hashtable1 PRIVATE list-pointer1 list-micro vm-v1 hashtable1 std-string-pointer1)
target_link_libraries(main-hashtable2 PRIVATE list-pointer2 list-micro hashtable2)
target_link_libraries(main-vm1 PRIVATE tests tests-vm-v1 list-alloc list-light vm-v1)
target_link_libraries(main-vm2 PRIVATE tests tests-vm-v2 list-alloc list-light vm-v2 list-pointer2 list-micro)
target_link_libraries(main-structs1 PRIVATE list-common)
target_link_libraries(main-structs2 PRIVATE list-class)
target_link_libraries(main-structs3 PRIVATE structs-v3)
target_link_libraries(main-linked-list PRIVATE stl-linked-list)
target_link_libraries(main-linked-list-static-enumerator PRIVATE stl-linked-list)
target_link_libraries(main-tests-vm1 PRIVATE list-micro list-pointer1 tests-vm-v1 vm-v1)
target_link_libraries(main-tests-vm2 PRIVATE list-micro list-pointer2 tests-vm-v2 vm-v2 list-pointer2 list-micro)

configure_file(all_english_words.txt ${CMAKE_BINARY_DIR}/all_english_words.txt COPYONLY)
configure_file(file.txt ${CMAKE_BINARY_DIR}/file.txt COPYONLY)

configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/src/compile_commands.json COPYONLY)
configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json COPYONLY)

FILE(WRITE ${CMAKE_BINARY_DIR}/input.txt
    "fopen<\"input.txt\",\"rb\">f (? f==0 perror<\"file not found\" exit<1 : print<\"all ok\")"
)

if(TARGETS)

    get_property(main_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    get_property(test_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

    list(FILTER main_target_names INCLUDE REGEX "^main-")
    list(FILTER test_target_names INCLUDE REGEX "^test-")

    string(REGEX REPLACE ";" " " main_target_names "${main_target_names}")
    string(REGEX REPLACE ";" " " test_target_names "${test_target_names}")

    FILE(WRITE ${CMAKE_BINARY_DIR}/targets.txt
        "${main_target_names} ${test_target_names}"
    )

    get_property(target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target_name ${target_names})
        get_target_property(TARGET_SOURCES ${target_name} SOURCES)
        set(output_dir "${CMAKE_BINARY_DIR}/${target_name}")
        file(MAKE_DIRECTORY ${output_dir})

        file(WRITE ${output_dir}/sources.txt "")
        foreach(source_file ${TARGET_SOURCES})
            file(APPEND ${output_dir}/sources.txt "${source_file}\n")
        endforeach()
    endforeach()

    file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.h" "examples/*.c" "examples/*.h")
    file(WRITE ${CMAKE_BINARY_DIR}/sources.txt "")
    foreach(source_file ${SOURCE_FILES})
        file(APPEND ${CMAKE_BINARY_DIR}/sources.txt "${source_file}\n")
    endforeach()

endif()
