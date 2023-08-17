#include "common/alloc.h"
#include "list-micro/data.h"
#include "list/v1/list_v1.h"
#include "playground/hashtable/v1/hashtable_v1.h"
#include "pointer/types/data/v1/data_v1.h"
#include "pointer/types/file/v1/file_v1.h"
#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/object/v1/object_v1.h"
#include "pointer/types/os/v1/os_v1.h"
#include "pointer/types/string/v1/string_v1.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/types/user/v1/user_v1.h"
#include "pointer/v1/pointer_v1.h"

/* definition */
extern const struct pointer_methods pointer_methods_definition;
extern const struct list_methods list_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct string_methods string_methods_definition;
extern const struct user_methods user_methods_definition;
extern const struct data_methods data_methods_definition;
extern const struct object_methods object_methods_definition;
extern const struct os_methods os_methods_definition;
extern const struct string_pointer_methods string_pointer_methods_definition;
extern const struct memory memory_definition;

/* definition */
static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct list_methods* list = &list_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct string_methods* string = &string_methods_definition;
static const struct user_methods* user = &user_methods_definition;
static const struct data_methods* data = &data_methods_definition;
static const struct object_methods* object = &object_methods_definition;
static const struct os_methods* os = &os_methods_definition;
static const struct string_pointer_methods* string_pointer = &string_pointer_methods_definition;
static const struct memory* memory = &memory_definition;

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    pointer->init(8);
    u64 list_ptr = list->alloc();
    u64 list_match_ptr = list->alloc();
    u64 string_ptr = string->load("a.bc.bb.ba.a");
    u64 pattern_ptr = string->load(".b");
    u64 string_pointer_ptr = 0;
    u64 match_ptr = string_ptr;
    u64 current_ptr = match_ptr;
    while ((string_pointer_ptr = string->strchr(current_ptr, pattern_ptr)) != 0) {
        list->push(list_ptr, string_pointer_ptr);
        match_ptr = string->match(string_pointer_ptr, pattern_ptr);
        list->push(list_match_ptr, match_ptr);
        current_ptr = match_ptr;
    }
#ifndef USE_GC
    u64 string_ptr1 = list->pop(list_ptr);
    u64 string_ptr2 = list->pop(list_ptr);
    u64 string_ptr3 = list->pop(list_ptr);
    u64 string_ptr4 = list->pop(list_ptr);
    u64 string_match_ptr1 = list->pop(list_match_ptr);
    u64 string_match_ptr2 = list->pop(list_match_ptr);
    u64 string_match_ptr3 = list->pop(list_match_ptr);
    string->free(string_ptr1);
    string->free(string_ptr2);
    string->free(string_ptr3);
    string->free(string_ptr4);
    string->free(string_match_ptr1);
    string->free(string_match_ptr2);
    string->free(string_match_ptr3);
    string->free(string_ptr);
    string->free(pattern_ptr);
    list->free(list_match_ptr);
    list->free(list_ptr);
#else
    pointer->gc();
#endif
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return 0;
}