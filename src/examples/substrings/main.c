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

static u64 load_data() {
    u64 list_ptr = list->alloc();
    u64 reversed_list_ptr = list->alloc();
    u64 file_path_ptr = string->load("c-substrings.txt");
    u64 mode_ptr = string->load("rb");
    u64 f_ptr = file->alloc(file_path_ptr, mode_ptr);
    if (f_ptr != 0) {
        u64 file_data_ptr = file->data(f_ptr);
        u64 size = data->size(file_data_ptr);
        if (size > 100) {
            size = 100;
        }
        file->free(f_ptr);
        u8* file_data = data->unsafe(file_data_ptr);
        u8* tmp = file_data;
        while (*tmp != 0 && size > 0) {
            while (*tmp != 0 && *tmp != '\n') {
                tmp++;
                size--;
            }
            *tmp++ = '\0';
            size--;
            u64 string_ptr = string->load((char*)file_data);
            list->push(list_ptr, string_ptr);
            file_data = tmp;
        }
        u64 data_ptr = 0;
        while ((data_ptr = list->pop(list_ptr)) != 0) {
            list->push(reversed_list_ptr, data_ptr);
        }
        list->free(list_ptr);
    }
    string->free(mode_ptr);
    return reversed_list_ptr;
}

static u64 read_input(const char* prompt) {
    char buffer[100];
    global_memset(&buffer, 0, 100);
    printf(">%s:\n", prompt);
    char ch = 0;
    for (int i = 0; i < 100; i++) {
        ch = (char)getchar();
        if (ch == EOF || ch == '\n') {
            break;
        }
        buffer[i] = ch;
    }
    u64 data_ptr = string->load((const char*)&buffer);
    return data_ptr;
}

static u64 read_data(u64 list_ptr, const char* prompt) {
    u64 data_ptr = 0;
    u64 ui_mode_ptr = string->load("UI_MODE");
    u64 mode_ptr = string->load("--ui");
    u64 value_ptr = os->getenv(ui_mode_ptr);
    if (ui_mode_ptr != 0 && string->strcmp(value_ptr, mode_ptr) != 0) {
        data_ptr = read_input(prompt);
    } else {
        printf(">%s:\n", prompt);
        data_ptr = list->pop(list_ptr);
        if (data_ptr == 0) {
            printf("\n");
        }
    }
    return data_ptr;
}

const char* ascii_code = "\x1b[1;32m";
const char* reset_code = "\x1b[0m";

static void launch_game(void) {
    u64 quit = 0;
    u64 answer = string->load("42");
    u64 message = string->load("it was a joke, now you will play in guess the number game!");
    os->putc(message);

    const char* prompt = "guess the number";

    while (quit == 0) {
        u64 data_ptr = read_input(prompt);
        if (string->strcmp(data_ptr, answer) != 0) {
            quit = 1;
            printf("%syou won!%s\n", ascii_code, reset_code);
            continue;
        }
        printf("%syou are wrong! try again!%s\n", ascii_code, reset_code);
    }
}

int main(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    pointer->init(8);
    u64 gc_ptr = list->alloc();
    if (string->gc(gc_ptr) == 0) {
        printf("no garbage collector for string type available\n");
        pointer->destroy();
        return 0;
    }
    u64 list_data_ptr = load_data();
    list->push(gc_ptr, list_data_ptr);
    u64 quit = 0;
    u64 secret_ptr = string->load("tic tak toe");
    while (quit == 0) {
        u64 string_ptr = read_data(list_data_ptr, "[string]");
        if (string->size(string_ptr) == 0) {
            quit = 1;
            printf(">[quit]\n");
            continue;
        }
        if (string->strcmp(string_ptr, secret_ptr) != 0) {
            printf(">[%sentering secret chat%s]\n", ascii_code, reset_code);
            launch_game();
            continue;
        }
        printf(">[%saccepted%s]:\n", ascii_code, reset_code);
        os->putc(string_ptr);
        u64 pattern_ptr = read_data(list_data_ptr, "[pattern]");
        if (string->size(pattern_ptr) == 0) {
            quit = 1;
            printf(">[quit]\n");
            continue;
        }
        printf(">[%saccepted%s]:\n", ascii_code, reset_code);
        os->putc(pattern_ptr);
        u64 size = string->size(pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = string->strchr(current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = string->match(string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            if (string->lessthan(string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = string->left(match_ptr, size);
                if (match_start_ptr == 0) {
                    break;
                }
                u64 distance = string->lessthan(string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                u64 str_ncpy = string->strncpy(match_start_ptr, size);
                if (str_ncpy == 0) {
                    break;
                }
                printf("%s[%lld]\n", string->unsafe(str_ncpy), distance);
            }
            current_ptr = match_ptr;
        }
    }
    printf(">[done]\n");
    list->release(gc_ptr);
#ifdef USE_GC
    pointer->gc();
#endif
    pointer->destroy();
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return 0;
}
