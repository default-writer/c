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
#include "pointer/types/string/v1/string_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct string_methods string_methods_definition;
void string_init();

/* definition */
extern void pointer_vm_register_type(const struct vm_type* type);
extern struct pointer_data vm_pointer;
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
static u64 string_alloc(void);
static void string_free(u64 ptr);
static void string_vm_free(struct pointer* ptr);
static u64 string_copy(u64 ptr);
static void string_strcpy(u64 dest_ptr, u64 src_ptr);
static void string_strcat(u64 dest_ptr, u64 src_ptr);
static u64 string_match_last(u64 src_ptr, u64 match_ptr);
static u64 string_load(const char* data);
static u64 string_getcwd(void);
static void string_printf(u64 ptr);
static void string_put_char(u64 ptr, char value);
static char* string_unsafe(u64 ptr);
static u64 string_size(u64 ptr);

/* implementation*/

struct list_handler {
    struct list_data* list;
};

static u64 string_alloc(void) {
    struct pointer* ptr = virtual->alloc(0, type->id);
    u64 data = vm->alloc(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void string_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return;
    }
    return string_vm_free(data_ptr);
}

static void string_vm_free(struct pointer* ptr) {
    virtual->free(ptr);
}

static u64 string_copy(u64 ptr) {
    const struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->size == 0) {
        return 0;
    }
    struct pointer* copy_ptr = virtual->alloc(data_ptr->size, type->id);
    memcpy(copy_ptr->data, data_ptr->data, copy_ptr->size); /* NOLINT */
    u64 data = vm->alloc(copy_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void string_strcpy(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = vm->read_type(dest, TYPE_STRING);
    if (dest_ptr == 0) {
        return;
    }
    const struct pointer* src_ptr = vm->read_type(src, TYPE_STRING);
    if (src_ptr == 0) {
        return;
    }
    if (src_ptr->size == 0) {
        return;
    }
    const char* data_src = src_ptr->data; /* NOLINT */
    if (dest_ptr->size == 0) {
        dest_ptr->data = global_alloc(src_ptr->size);
        dest_ptr->size = src_ptr->size;
    } else {
        u64 size = src_ptr->size + 1;
        if (dest_ptr->size < size) {
            virtual->realloc(dest_ptr, size);
        }
    }
    char* data_dest = dest_ptr->data;
    strcpy(data_dest, data_src); /* NOLINT */
}

static void string_strcat(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = vm->read_type(dest, TYPE_STRING);
    if (dest_ptr == 0) {
        return;
    }
    const struct pointer* src_ptr = vm->read_type(src, TYPE_STRING);
    if (src_ptr == 0) {
        return;
    }
    if (src_ptr->size == 0) {
        return;
    }
    const char* data_src = src_ptr->data; /* NOLINT */
    if (dest_ptr->size == 0) {
        dest_ptr->data = global_alloc(src_ptr->size);
        dest_ptr->size = src_ptr->size;
    } else {
        u64 size = dest_ptr->size + src_ptr->size - 1;
        if (dest_ptr->size < size) {
            virtual->realloc(dest_ptr, size);
        }
    }
    char* data_dest = dest_ptr->data;
    strcat(data_dest, data_src); /* NOLINT */
}

static u64 string_match_last(u64 src, u64 match) {
    const struct pointer* src_ptr = vm->read_type(src, TYPE_STRING);
    if (src_ptr == 0) {
        return 0;
    }
    const struct pointer* match_ptr = vm->read_type(match, TYPE_STRING);
    if (match_ptr == 0) {
        return 0;
    }
    const char* data_src = src_ptr->data;
    const char* data_match = match_ptr->data;
    char* data_last = strrchr(data_src, *data_match);
    while (data_last != 0 && *data_last != 0 && *data_match != 0 && *data_last == *data_match) {
        data_last++;
        data_match++;
    }
    if (data_last == 0) {
        return 0;
    }
    if (*data_match != 0) {
        return 0;
    }
    struct pointer* last_match_ptr = virtual->alloc(0, type->id);
    last_match_ptr->data = --data_last;
    u64 data = vm->alloc(last_match_ptr);
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
    struct pointer* data_ptr = virtual->alloc(size, type->id);
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
    char* data = global_alloc(size);
    strcpy(data, cwd); /* NOLINT */
    data_ptr = string_load(data);
    global_free(data, size);
    return data_ptr;
}

static void string_printf(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return;
    }
    const char* data = data_ptr->data;
    if (data == 0) {
        return;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    void* ptr_data = data_ptr->data;
    printf("   .: %016llx > %016llx\n", (u64)data_ptr, (u64)ptr_data);
#endif
    puts(data);
}

static void string_put_char(u64 ptr, char value) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return;
    }
    char* data = data_ptr->data;
    if (data == 0) {
        return;
    }
    if (value == 0) {
        return;
    }
    *data = value;
}

static char* string_unsafe(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    char* data = data_ptr->data;
    return data;
}

static u64 string_size(u64 ptr) {
    const struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = data_ptr->size;
    return size;
}

static const struct vm_type type_definition = {
    .free = string_vm_free,
    .id = TYPE_STRING
};

static void INIT init() {
    pointer_vm_register_type(type);
}

/* public */
const struct string_methods string_methods_definition = {
    .alloc = string_alloc,
    .free = string_free,
    .copy = string_copy,
    .strcpy = string_strcpy,
    .strcat = string_strcat,
    .match_last = string_match_last,
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