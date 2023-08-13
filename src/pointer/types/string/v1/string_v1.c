/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "string_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING;

/* api */
const struct string_methods string_methods_definition;
void string_init();

/* definition */
extern u64 pointer_vm_register_type(u64 id, const struct vm_type* type);
extern struct pointer_data vm_pointer;
extern const struct string_pointer_methods string_pointer_methods_definition;
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;
static struct pointer_data* base = &vm_pointer;
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static char* string_pointer_internal(struct pointer* data_ptr, u64* data_size, u64* data_offset);

/* declaration */
static void string_free(u64 ptr);
static void string_vm_free(struct pointer* ptr);
static u64 string_copy(u64 ptr);
static void string_strcpy(u64 dest_ptr, u64 src_ptr);
static void string_strcat(u64 dest_ptr, u64 src_ptr);
static u64 string_match_last_src(u64 src_ptr, u64 match_ptr);
static u64 string_load(const char* data);
static u64 string_getcwd(void);
static void string_printf(u64 ptr);
static void string_put_char(u64 ptr, char value);
static char* string_unsafe(u64 ptr);
static u64 string_size(u64 ptr);

/* definition */
extern const struct memory memory_definition;
static const struct memory* memory = &memory_definition;

struct list_handler {
    struct list_data* list;
};

static char* string_pointer_internal(struct pointer* data_ptr, u64* data_size, u64* data_offset) {
    u64 offset = 0;
    struct pointer* ptr = 0;
    if (data_ptr->id == TYPE_STRING) {
        ptr = data_ptr;
    }
    if (data_ptr->id == TYPE_STRING_POINTER) {
        struct string_reference* ref = data_ptr->data;
        ptr = vm->read_type(ref->address, TYPE_STRING);
        if (ptr == 0) {
            return 0;
        }
        offset = ref->offset;
    }
    if (ptr == 0) {
        return 0;
    }
    *data_size = ptr->size;
    *data_offset = offset;
    char* data = ptr->data;
    return data;
}

static void string_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    return string_vm_free(data_ptr);
}

static void string_vm_free(struct pointer* ptr) {
    virtual->free(ptr);
}

static u64 string_copy(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(data_ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    data += offset;
    struct pointer* copy_ptr = virtual->alloc(size - offset, id);
    memcpy(copy_ptr->data, data, size - offset); /* NOLINT */
    u64 copy = vm->alloc(copy_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)copy);
#endif
    return copy;
}

static void string_strcpy(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = vm->read_type(dest, id);
    if (dest_ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(src);
    if (data_ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(data_ptr, &size, &offset);
    if (data == 0) {
        return;
    }
    data += offset;
    if (dest_ptr->size < size) {
        virtual->realloc(dest_ptr, size);
    }
    char* data_dest = dest_ptr->data;
    strcpy(data_dest, data); /* NOLINT */
}

static void string_strcat(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = vm->read_type(dest, id);
    if (dest_ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(src);
    if (data_ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(data_ptr, &size, &offset);
    if (data == 0) {
        return;
    }
    data += offset;
    const char* data_src = data; /* NOLINT */
    size += dest_ptr->size - 1;
    if (dest_ptr->size < size) {
        virtual->realloc(dest_ptr, size);
    }
    char* data_dest = dest_ptr->data;
    strcat(data_dest, data_src); /* NOLINT */
}

/*
    str1                         str2
    0xdeaddeaddeaddeadbeef <---> 0xdead
                  ^  ^
                  |  |
                  |  d is the last matching character of str1 string matching str2
                  | d is the rightmost character of str1 string matching str2 (strrchr)
*/
static u64 string_match_last_src(u64 src, u64 match) {
    struct pointer* src_ptr = vm->read_type(src, id);
    if (src_ptr == 0) {
        return 0;
    }
    const struct pointer* match_ptr = vm->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    char* data_src = src_ptr->data;
    char* data_match = match_ptr->data;
    char* str1 = strrchr(data_src, *data_match);
    if (str1 == 0) {
        return 0;
    }
    u64 offset = (u64)((u8*)str1 - (u8*)data_src);
    char* str2 = data_match;
    while (*str1 != 0 && *str2 != 0 && offset < src_ptr->size) {
        if (*str1 != *str2) {
            break;
        }
        str1++;
        str2++;
        offset++;
    }
    if (*str1 == 0 && *str2 != 0) {
        return 0;
    }
    struct pointer* data_ptr = virtual->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = data_ptr->data;
    ref->address = src;
    ref->offset = offset;
    u64 data = vm->alloc(data_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 string_load(const char* src_data) {
    if (src_data == 0) {
        return 0;
    }
    if (*src_data == 0) {
        return 0;
    }
    u64 size = strlen(src_data) + 1;
    struct pointer* data_ptr = virtual->alloc(size, id);
    memcpy(data_ptr->data, src_data, size); /* NOLINT */
    u64 data = vm->alloc(data_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 string_getcwd(void) {
    char cwd[PATH_MAX];
    u64 data_ptr;
    getcwd(cwd, sizeof(cwd));
    u64 size = strlen(cwd) + 1;
    char* data = memory->alloc(size);
    strcpy(data, cwd); /* NOLINT */
    data_ptr = string_load(data);
    memory->free(data, size);
    return data_ptr;
}

static void string_printf(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* data = string_pointer_internal(data_ptr, &size, &offset);
    if (data == 0) {
        return;
    }
    data += offset;
#ifdef USE_MEMORY_DEBUG_INFO
    void* ptr_data = data_ptr->data;
    printf("   .: %016llx > %016llx\n", (u64)data_ptr, (u64)ptr_data);
#endif
    puts(data);
}

static void string_put_char(u64 src, char value) {
    struct pointer* data_ptr = vm->read(src);
    if (data_ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(data_ptr, &size, &offset);
    if (data == 0) {
        return;
    }
    data += offset;
    *data = value;
}

static char* string_unsafe(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    char* data = data_ptr->data;
    return data;
}

static u64 string_size(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(data_ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    return size;
}

static const struct vm_type type_definition = {
    .free = string_vm_free
};

static void INIT init() {
    pointer_vm_register_type(id, type);
}

/* public */
const struct string_methods string_methods_definition = {
    .free = string_free,
    .copy = string_copy,
    .strcpy = string_strcpy,
    .strcat = string_strcat,
    .match_last_src = string_match_last_src,
    .load = string_load,
    .getcwd = string_getcwd,
    .printf = string_printf,
    .put_char = string_put_char,
    .unsafe = string_unsafe,
    .size = string_size
};

#ifndef ATTRIBUTE
void string_init() {
    init();
}
#endif
