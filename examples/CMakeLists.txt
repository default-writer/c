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

include_directories(. ../src)

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

set(LIB_CLASS_SOURCE class/class.c)

set(LIB_COMMON_SOURCE common/memory.c)
set(LIB_LCG_SOURCE common/lcg.c)
set(LIB_PARAMETERS_SOURCE common/parameters.c)

set(LIB_LIST1_SOURCE playground/list/v1/list_v1.c)
set(LIB_LIST2_SOURCE playground/list/v2/list_v2.c)
set(LIB_VM1_SOURCE pointer/v1/virtual_v1.c)
set(LIB_VM2_SOURCE vm/v2/vm_v2.c)
set(LIB_HASHTABLE1_SOURCE playground/hashtable/v1/hashtable_v1.c)
set(LIB_HASHTABLE2_SOURCE playground/hashtable/v2/hashtable_v2.c)
set(LIB_STANDARD_SOURCE list/v1/list_v1.c)
set(LIB_LIGHT_SOURCE list-light/code.c)
set(LIB_MICRO_SOURCE list-micro/code.c)
set(LIB_ALLOC_SOURCE list-alloc/code.c)
set(LIB_EXPERIMENTAL_SOURCE list-experimental/code.c)
set(LIB_CLASS1_SOURCE playground/class/v1/class_v1.c)
set(LIB_CLASS2_SOURCE playground/class/v2/class_v2.c)
set(LIB_MEMORY1_SOURCE
    playground/memory/v1/memory_v1.c 
    playground/memory/v2/memory_v2.c
    playground/memory/v3/memory_v3.c)
set(LIB_MEMORY2_SOURCE 
    playground/memory/v4/memory_v4.c)
set(LIB_MEMORY3_SOURCE
    playground/memory/v5/memory_v5.c)
set(LIB_MEMORY4_SOURCE
    playground/memory/v6/memory_v6.c)
set(LIB_BRAIN_SOURCE playground/brain/brain.c)
set(LIB_POINTER2_SOURCE pointer/v2/pointer_v2.c)
set(LIB_MEMORY_LIST1_SOURCE playground/memory/list/v1/memory_list_v1.c)
set(LIB_MEMORY_LIST2_SOURCE playground/memory/list/v2/memory_list_v2.c)
set(LIB_MEMORY_LIST3_SOURCE playground/memory/list/v3/memory_list_v3.c)
set(LIB_MEMORY_REF1_SOURCE memory/v1/ref_v1.c)
set(LIB_MEMORY_REF2_SOURCE memory/v2/ref_v2.c)
set(LIB_MEMORY_REF3_SOURCE memory/v3/ref_v3.c)

set(LIB_LINKED_LIST_SOURCE
    stl/linked_list.c
    stl/linked_list_enumerator.c
    stl/linked_list_static_enumerator.c)

# test
set(TEST_POINTER_SOURCE tests/test-pointer.c)
set(TEST_HASHES_SOURCE tests/test-hashes.c)
set(TEST_LINKED_LIST_SOURCE tests/main-linked-list.c)
set(TEST_LINKED_LIST_STATIC_ENUMERATOR_SOURCE tests/main-linked-list-static-enumerator.c)

set(TESTS_SRC_LIST tests/src/test_list/test_list.c)
set(TESTS_SRC_LIST_MICRO tests/src/test_list-micro/test_list-micro.c)
set(TESTS_SRC_LIST_ALLOC tests/src/test_list-alloc/test_list-alloc.c)
set(TESTS_SRC_LIST_EXPERIMENTAL tests/src/test_list-experimental/test_list-experimental.c)
set(TESTS_SRC_VM_V1 tests/src/test_vm/v1/test_vm_v1.c)
set(TESTS_SRC_VM_V2 tests/src/test_vm/v2/test_vm_v2.c)
set(TESTS_SRC_POINTER tests/src/test_pointer/test_pointer.c)

# main
set(MAIN_STANDARD_SOURCE tests/main-default.c)
set(MAIN_LIGHT_SOURCE tests/main-light.c ${LIB_LIGHT_SOURCE})
set(MAIN_MICRO_SOURCE tests/main-micro.c)
set(MAIN_ALLOC_SOURCE tests/main-alloc.c)
set(MAIN_EXPERIMENTAL_SOURCE tests/main-experimental.c)
set(MAIN_PLAYGROUND1_SOURCE tests/v1/main-playground_v1.c ${LIB_CLASS1_SOURCE})
set(MAIN_PLAYGROUND2_SOURCE tests/v2/main-playground_v2.c ${LIB_CLASS2_SOURCE})
set(MAIN_MEMORY_SOURCE tests/main-memory.c ${LIB_MEMORY1_SOURCE})
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
set(MAIN_TESTS_VM1_SOURCE tests/v1/main-tests_v1.c)
set(MAIN_TESTS_VM2_SOURCE tests/v2/main-tests_v2.c)

# zen
set(ZEN_SOURCE zen/zen.c)

set_source_files_properties(${GCC_NASM_SOURCE} PROPERTIES LANGUAGE ASM_NASM)

add_library(demo-gcc-nasm OBJECT ${GCC_NASM_SOURCE})
add_library(structs-v3 STATIC ${TEST_STRUCTS3_LIB_SOURCE})
add_library(list-class STATIC ${LIB_CLASS_SOURCE})
add_library(tests STATIC
    ${TESTS_SRC_LIST}
    ${TESTS_SRC_LIST_ALLOC}
    ${TESTS_SRC_LIST_EXPERIMENTAL}
    ${TESTS_SRC_LIST_MICRO}
    ${TESTS_SRC_POINTER}
)
add_library(tests-vm-v2 STATIC ${TESTS_SRC_VM_V2})

# all
add_library(list-v1 STATIC ${LIB_LIST1_SOURCE})
add_library(list-v2 STATIC ${LIB_LIST2_SOURCE})
add_library(vm-v2 STATIC ${LIB_VM2_SOURCE})
add_library(hashtable1 ${LIB_HASHTABLE1_SOURCE})
add_library(hashtable2 ${LIB_HASHTABLE2_SOURCE})
add_library(list-memory1 STATIC ${LIB_MEMORY1_SOURCE})
add_library(list-memory2 STATIC ${LIB_MEMORY2_SOURCE})
add_library(list-memory3 STATIC ${LIB_MEMORY3_SOURCE})
add_library(list-memory4 STATIC ${LIB_MEMORY4_SOURCE})
add_library(list-lcg STATIC ${LIB_LCG_SOURCE})
add_library(list-parameters STATIC ${LIB_PARAMETERS_SOURCE})
add_library(list-default STATIC ${LIB_STANDARD_SOURCE})
add_library(list-light STATIC ${LIB_LIGHT_SOURCE})
add_library(list-alloc STATIC ${LIB_ALLOC_SOURCE})
add_library(list-experimental STATIC ${LIB_EXPERIMENTAL_SOURCE})
add_library(list-class1 STATIC ${LIB_CLASS1_SOURCE})
add_library(list-class2 STATIC ${LIB_CLASS2_SOURCE})
add_library(list-brain STATIC ${LIB_BRAIN_SOURCE})
add_library(list-pointer2 STATIC ${LIB_POINTER2_SOURCE})
add_library(memory-list1 STATIC ${LIB_MEMORY_LIST1_SOURCE})
add_library(memory-list2 STATIC ${LIB_MEMORY_LIST2_SOURCE})
add_library(memory-list3 STATIC ${LIB_MEMORY_LIST3_SOURCE})
add_library(memory-ref1 STATIC ${LIB_MEMORY_REF1_SOURCE})
add_library(memory-ref2 STATIC ${LIB_MEMORY_REF2_SOURCE})
add_library(memory-ref3 STATIC ${LIB_MEMORY_REF3_SOURCE})
add_library(stl-linked-list STATIC ${LIB_LINKED_LIST_SOURCE})

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
add_executable(test-hashes ${TEST_HASHES_SOURCE})
add_executable(test-pointer ${TEST_POINTER_SOURCE})
add_executable(main-default ${MAIN_STANDARD_SOURCE})
add_executable(main-light ${MAIN_LIGHT_SOURCE})
add_executable(main-micro ${MAIN_MICRO_SOURCE})
add_executable(main-alloc ${MAIN_ALLOC_SOURCE})
add_executable(main-experimental ${MAIN_EXPERIMENTAL_SOURCE})
add_executable(main-playground1 ${MAIN_PLAYGROUND1_SOURCE})
add_executable(main-playground2 ${MAIN_PLAYGROUND2_SOURCE})
add_executable(main-memory ${MAIN_MEMORY_SOURCE})
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
add_executable(main-linked-list ${TEST_LINKED_LIST_SOURCE})
add_executable(main-linked-list-static-enumerator ${TEST_LINKED_LIST_STATIC_ENUMERATOR_SOURCE})
add_executable(main-tests-vm2 ${MAIN_TESTS_VM2_SOURCE})

# examples
# zen
add_executable(zen ${ZEN_SOURCE})

set_target_properties(demo-gcc PROPERTIES NASM_OBJ_FORMAT elf64)

target_link_libraries(c-substrings PRIVATE ctypes)
target_link_libraries(demo-url PRIVATE curl ctypes)
target_link_libraries(main-private-structs1 PRIVATE list-common)
target_link_libraries(main-private-structs2 PRIVATE list-common)
target_link_libraries(main-private-structs3 PRIVATE list-common)
target_link_libraries(main-private-structs4 PRIVATE list-common)
target_link_libraries(main-structs1 PRIVATE list-common)
target_link_libraries(main-structs2 PRIVATE list-class)
target_link_libraries(main-structs3 PRIVATE structs-v3)
target_link_libraries(structs-v3 PRIVATE list-class)
target_link_libraries(list-class PRIVATE list-common)
target_link_libraries(hashtable1 PRIVATE list-lcg)
target_link_libraries(hashtable2 PRIVATE list-lcg)
target_link_libraries(list-memory1 PRIVATE list-common)
target_link_libraries(list-memory2 PRIVATE list-common)
target_link_libraries(list-memory3 PRIVATE list-common)

target_link_libraries(tests PRIVATE list-alloc list-parameters list-micro)
target_link_libraries(tests-vm-v2 PRIVATE tests list-common list-micro list-pointer2)
target_link_libraries(list-alloc PRIVATE list-parameters)
target_link_libraries(list-pointer2 PRIVATE list-common)
target_link_libraries(list-experimental PRIVATE list-parameters)
target_link_libraries(memory-ref1 PRIVATE memory-list1 list-micro list-common)
target_link_libraries(memory-ref2 PRIVATE memory-list2 list-micro list-common)
target_link_libraries(memory-ref3 PRIVATE memory-list3 list-micro list-common)
target_link_libraries(list-pointer2 PRIVATE vm-v2)
target_link_libraries(stl-linked-list PRIVATE list-common)
target_link_libraries(test-pointer PRIVATE tests list-default tests-vm-v1 ctypes)
target_link_libraries(main-default PRIVATE tests list-common list-alloc list-v1 list-default)
target_link_libraries(main-light PRIVATE tests list-common list-alloc  list-micro list-light)
target_link_libraries(main-micro PRIVATE tests list-common list-alloc list-micro)
target_link_libraries(main-alloc PRIVATE tests list-common list-alloc list-lcg list-parameters)
target_link_libraries(main-experimental PRIVATE tests list-common list-experimental list-lcg list-parameters)
target_link_libraries(main-playground1 PRIVATE list-default list-class1 list-micro list-common list-v1)
target_link_libraries(main-playground2 PRIVATE list-class2 list-micro list-common list-v2)
target_link_libraries(main-memory PRIVATE list-memory1 list-memory2 memory-ref1)
target_link_libraries(main-memory3 PRIVATE list-memory3 memory-ref2)
target_link_libraries(main-memory4 PRIVATE list-memory4 memory-ref3 list-lcg)
target_link_libraries(main-brain1 PRIVATE tests list-default tests-vm-v1 list-micro ctypes)
target_link_libraries(main-brain2 PRIVATE tests list-default tests-vm-v2 list-micro vm-v2)
target_link_libraries(main-pointer1 PRIVATE list-micro ctypes)
target_link_libraries(main-pointer2 PRIVATE list-pointer2 list-micro)
target_link_libraries(main-hashtable1 PRIVATE list-micro hashtable1 ctypes)
target_link_libraries(main-hashtable2 PRIVATE list-pointer2 list-micro hashtable2)
target_link_libraries(main-vm1 PRIVATE tests tests-vm-v1 list-alloc list-light ctypes)
target_link_libraries(main-vm2 PRIVATE tests tests-vm-v2 list-alloc list-light vm-v2 list-pointer2 list-micro)
target_link_libraries(main-linked-list PRIVATE stl-linked-list)
target_link_libraries(main-linked-list-static-enumerator PRIVATE stl-linked-list)
target_link_libraries(main-tests-vm1 PRIVATE list-micro tests-vm-v1 ctypes)
target_link_libraries(main-tests-vm2 PRIVATE tests tests-vm-v2 list-micro list-pointer2 vm-v2 list-pointer2 list-micro)

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


if(TARGETS)

    get_property(main_target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)

    list(FILTER main_target_names INCLUDE REGEX "^main-")

    string(REGEX REPLACE ";" " " main_target_names "${main_target_names}")

    FILE(WRITE ${CMAKE_BINARY_DIR}/targets.txt
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
