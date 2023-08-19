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
    u64 quit = 0;
    char buffer[100];
    char ch = 0;
    while (quit == 0) {
        memset(&buffer, 0, 100);
        for (int i = 0; i < 100; i++) {
            ch = (char)getchar();
            if (ch == EOF || ch == '\n') {
                break;
            }
            buffer[i] = ch;
        }
        u64 string_ptr = string->load((const char*)&buffer);
        if (string->size(string_ptr) == 0) {
            quit = 1;
            continue;
        }
        os->putc(string_ptr);
        memset(&buffer, 0, 100);
        for (int i = 0; i < 100; i++) {
            ch = (char)getchar();
            if (ch == EOF || ch == '\n') {
                break;
            }
            buffer[i] = ch;
        }
        u64 pattern_ptr = string->load((const char*)&buffer);
        if (string->size(pattern_ptr) == 0) {
            quit = 1;
            continue;
        }
        os->putc(pattern_ptr);
#ifndef USE_GC
        u64 gc = list->alloc();
#endif
#ifndef USE_GC
        list->push(gc, string_ptr);
#endif
#ifndef USE_GC
        list->push(gc, pattern_ptr);
#endif
        u64 size = string->size(pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        os->putc(string_ptr);
        while ((string_pointer_ptr = string->strchr(current_ptr, pattern_ptr)) != 0) {
#ifndef USE_GC
            list->push(gc, string_pointer_ptr);
#endif
            u64 match_ptr = string->match(string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            u64 match_start_ptr = string->left(match_ptr, size);
            if (match_start_ptr == 0) {
                continue;
            }
            u64 str_ncpy = string->strncpy(match_start_ptr, size);
            if (str_ncpy == 0) {
                continue;
            }
            u64 distance = string->diff(string_ptr, match_start_ptr);
            if (distance == 0) {
                continue;
            }
            u64 i = 0;
            while (i++ < distance - 1) {
                printf(" ");
            }
            os->putc(str_ncpy);
            printf("match found at index %lld\n", distance - 1);
#ifndef USE_GC
            list->push(gc, match_ptr);
            list->push(gc, match_start_ptr);
            list->push(gc, str_ncpy);
#endif
            current_ptr = match_ptr;
        }
#ifndef USE_GC
        u64 ptr;
        while ((ptr = list->pop(gc)) != 0) {
            string->free(ptr);
        }
        list->free(gc);
#else
        pointer->gc();
#endif
    }
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return 0;
}