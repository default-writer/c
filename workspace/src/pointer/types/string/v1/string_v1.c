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
#include "common/memory.h"

#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/string_pointer/v1/string_pointer_v1.h"
#include "pointer/types/types.h"
#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STRING;

/* api */
const struct string_methods string_methods_definition;

#ifndef ATTRIBUTE
void string_init(void);
#endif

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static char* string_pointer_internal(struct pointer* data_ptr, u64* data_size, u64* data_offset);
static char* string_strrchr_internal(char* data, char* str2, u64 size, u64 offset);
static char* string_strchr_internal(char* data, char* str2, u64 size, u64 offset);

/* definition */
static void string_free(u64 ptr);
static void string_vm_free(struct pointer* ptr);
static u64 string_copy(u64 ptr);
static void string_strcpy(u64 dest_ptr, u64 src_ptr);
static void string_strcat(u64 dest_ptr, u64 src_ptr);
static u64 string_strrchr(u64 src_ptr, u64 match_ptr);
static u64 string_strchr(u64 src_ptr, u64 match_ptr);
static u64 string_match(u64 src_ptr, u64 match_ptr);
static u64 string_offset(u64 src_ptr, u64 match_ptr);
static u64 string_load(const char* data);
static void string_put_char(u64 ptr, char value);
static char* string_unsafe(u64 ptr);
static u64 string_size(u64 ptr);
static u64 string_lessthan(u64 src, u64 dst);
static u64 string_greaterthan(u64 src, u64 dst);
static u64 string_equals(u64 src, u64 dst);
static u64 string_compare(u64 src, u64 dst);
static u64 string_left(u64 src, u64 shift);
static u64 string_strncpy(u64 src, u64 nbytes);
static u64 string_left_strncpy(u64 src, u64 nbytes);
static u64 string_right(u64 src, u64 shift);
static u64 string_move_left(u64 src, u64 shift);
static u64 string_move_right(u64 src, u64 shift);
static u64 string_strcmp(u64 src, u64 dest);

/* definition */
extern const struct list_methods list_methods_definition;

/* definition */
static const struct list_methods* list_type = &list_methods_definition;

/* definition */
struct list_handler {
    struct list_data* list;
};

/* implementation */
static char* string_pointer_internal(struct pointer* data_ptr, u64* data_size, u64* data_offset) {
    u64 offset = 0;
    struct pointer* ptr = 0;
    if (!pointer->read_type(data_ptr, TYPE_STRING) && !pointer->read_type(data_ptr, TYPE_STRING_POINTER)) {
        return 0;
    }
    if (pointer->read_type(data_ptr, TYPE_STRING)) {
        ptr = data_ptr;
    }
    if (pointer->read_type(data_ptr, TYPE_STRING_POINTER)) {
        struct string_reference* ref = pointer->read(data_ptr);
        while ((ptr = virtual->read(ref->address)) != 0) {
            if (!pointer->read_type(ptr, TYPE_STRING_POINTER)) {
                break;
            }
            offset += ref->offset;
            ref = pointer->read(ptr);
        }
        ptr = virtual->read_type(ref->address, TYPE_STRING);
        if (ptr == 0) {
            return 0;
        }
        offset += ref->offset;
    }
    *data_size = pointer->size(ptr);
    *data_offset = offset;
    char* data = pointer->read(ptr);
    return data;
}

static char* string_strrchr_internal(char* data, char* str2, u64 size, u64 offset) {
    char* str1 = data;
    char* ptr = str1;
    char* last = 0;
    u64 rsize = size - 1;
    while (rsize > offset && *ptr != 0) {
        if (*ptr == *str2) {
            last = ptr;
        }
        ++ptr;
        rsize--;
    }
    if (last == 0) {
        return 0;
    }
    str1 = last;
    return str1;
}

static char* string_strchr_internal(char* data, char* str2, u64 size, u64 offset) {
    char* str1 = data;
    char* ptr = str1;
    char* last = 0;
    u64 rsize = size - 1;
    while (rsize > offset && *ptr != 0 && last == 0) {
        if (*ptr == *str2) {
            last = ptr;
        }
        ++ptr;
        rsize--;
    }
    if (last == 0) {
        return 0;
    }
    str1 = last;
    return str1;
}

/* api */
static void string_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    if (pointer->read_type(data_ptr, TYPE_STRING)) {
        string_vm_free(data_ptr);
        return;
    }
    if (pointer->read_type(data_ptr, TYPE_STRING_POINTER)) {
        struct string_reference* ref = pointer->read(data_ptr);
        struct pointer* pointer_ptr = virtual->read_type(ref->address, TYPE_STRING_POINTER);
        string_vm_free(pointer_ptr);
        string_vm_free(data_ptr);
        return;
    }
}

static void string_vm_free(struct pointer* ptr) {
    pointer->release(ptr);
}

static u64 string_copy(u64 src) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    data += offset;
    struct pointer* data_ptr = pointer->alloc(size - offset, id);
    memcpy(pointer->read(data_ptr), data, size - offset); /* NOLINT */
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static void string_strcpy(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = virtual->read_type(dest, id);
    if (dest_ptr == 0) {
        return;
    }
    struct pointer* src_ptr = virtual->read(src);
    if (src_ptr == 0) {
        return;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    char* data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (data == 0) {
        return;
    }
    data += src_offset;
    u64 dest_size = pointer->size(dest_ptr);
    if (dest_size < src_size) {
        pointer->realloc(dest_ptr, src_size);
    }
    char* data_dest = pointer->read(dest_ptr);
    strcpy(data_dest, data); /* NOLINT */
}

static void string_strcat(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    struct pointer* dest_ptr = virtual->read_type(dest, id);
    if (dest_ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(src);
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
    u64 dest_size = pointer->size(dest_ptr);
    size += dest_size - 1;
    if (dest_size < size) {
        pointer->realloc(dest_ptr, size);
    }
    char* data_dest = pointer->read(dest_ptr);
    strcat(data_dest, data_src); /* NOLINT */
}

static u64 string_strrchr(u64 src, u64 match) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    const struct pointer* match_ptr = virtual->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    char* text = data + offset;
    char* str2 = pointer->read(match_ptr);
    char* str1 = string_strrchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        return 0;
    }
    data += offset;
    offset = (u64)(str1 - data);
    struct pointer* data_ptr = pointer->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = pointer->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_strchr(u64 src, u64 match) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    const struct pointer* match_ptr = virtual->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    char* text = data + offset;
    char* str2 = pointer->read(match_ptr);
    char* str1 = string_strchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        return 0;
    }
    data += offset;
    offset = (u64)(str1 - data);
    struct pointer* data_ptr = pointer->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = pointer->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_match(u64 src, u64 match) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    const struct pointer* match_ptr = virtual->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    data += offset;
    u64 match_size = pointer->size(match_ptr);
    char* str2 = pointer->read(match_ptr);
    char* str1 = data;
    char* ptr2 = str2;
    size -= offset;
    while (size > 0) {
        while (size > 0 && *str1 != 0 && *str1 != *str2) {
            str1++;
            size--;
        }
        if (*str1 == 0) {
            return 0;
        }
        if (size < match_size) {
            return 0;
        }
        char* ptr1 = str1;
        while (*str1 != 0 && *str2 != 0) {
            if (*str1 != *str2) {
                str1 = ++ptr1;
                size--;
                break;
            }
            str1++;
            str2++;
        }
        if (*str1 == 0 || *str2 == 0) {
            break;
        }
        str2 = ptr2;
    }
    offset = (u64)(str1 - data);
    struct pointer* data_ptr = pointer->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = pointer->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_offset(u64 src, u64 match) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    const struct pointer* match_ptr = virtual->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    data += offset;
    u64 match_size = pointer->size(match_ptr);
    char* str2 = pointer->read(match_ptr);
    char* str1 = data;
    char* pos = str1;
    char* last_match = 0;
    u64 orig_size = size - offset;
    while (orig_size > 0 && *pos != 0) {
        if (*pos == *str2) {
            last_match = pos;
        }
        ++pos;
    }
    size = orig_size;
    str1 = last_match;
    if (str1 == 0) {
        return 0;
    }
    offset = (u64)(str1 - data);
    char* ptr2 = str2;
    size -= offset;
    while (size > 0) {
        while (size > 0 && *str1 != 0 && *str1 != *str2) {
            str1++;
            size--;
        }
        if (*str1 == 0) {
            return 0;
        }
        if (size < match_size) {
            return 0;
        }
        char* ptr1 = str1;
        while (*str1 != 0 && *str2 != 0) {
            if (*str1 != *str2) {
                str1 = ++ptr1;
                size--;
                break;
            }
            str1++;
            str2++;
        }
        if (*str1 == 0 || *str2 == 0) {
            break;
        }
        str2 = ptr2;
    }
    offset = (u64)(str1 - data);
    struct pointer* data_ptr = pointer->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = pointer->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_load(const char* src_data) {
    if (src_data == 0) {
        return 0;
    }
    if (*src_data == 0) {
        return 0;
    }
    u64 size = strlen(src_data) + 1;
    struct pointer* data_ptr = pointer->alloc(size, id);
    memcpy(pointer->read(data_ptr), src_data, size); /* NOLINT */
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static void string_put_char(u64 src, char value) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return;
    }
    data += offset;
    *data = value;
}

static char* string_unsafe(u64 src) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    data += offset;
    return data;
}

static u64 string_size(u64 src) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    return size - 1;
}

static u64 string_lessthan(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    struct pointer* src_ptr = virtual->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    struct pointer* dst_ptr = virtual->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    if (src_offset >= dst_offset) {
        return 0;
    }
    return (u64)(dst_offset - src_offset);
}

static u64 string_greaterthan(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    struct pointer* src_ptr = virtual->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    struct pointer* dst_ptr = virtual->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    if (src_offset <= dst_offset) {
        return 0;
    }
    return (u64)(src_offset - dst_offset);
}

static u64 string_equals(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    struct pointer* src_ptr = virtual->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    struct pointer* dst_ptr = virtual->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    if (src_offset != dst_offset) {
        return 0;
    }
    return (u64)(0 - 1);
}

static u64 string_compare(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    struct pointer* src_ptr = virtual->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    struct pointer* dst_ptr = virtual->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    return (u64)(0 - 1);
}

static u64 string_left(u64 src, u64 shift) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    if (offset < shift) {
        return 0;
    }
    struct pointer* data_ptr = pointer->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = pointer->read(data_ptr);
    ref->address = src;
    ref->offset = 0 - shift;
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_strncpy(u64 src, u64 nbytes) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    if (offset + nbytes >= size) {
        return 0;
    }
    data += offset;
    struct pointer* data_ptr = pointer->alloc(nbytes + 1, id);
    memcpy(pointer->read(data_ptr), data, nbytes); /* NOLINT */
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_left_strncpy(u64 src, u64 nbytes) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    if (offset < nbytes) {
        return 0;
    }
    data += (offset - nbytes);
    struct pointer* data_ptr = pointer->alloc(nbytes + 1, id);
    memcpy(pointer->read(data_ptr), data, nbytes); /* NOLINT */
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_right(u64 src, u64 nbytes) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    if (offset + nbytes >= size) {
        return 0;
    }
    struct pointer* data_ptr = pointer->alloc(sizeof(struct string_reference), TYPE_STRING_POINTER);
    struct string_reference* ref = pointer->read(data_ptr);
    ref->address = src;
    ref->offset = nbytes;
    u64 result = virtual->alloc(data_ptr);
    return result;
}

static u64 string_move_left(u64 src, u64 nbytes) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    if (!pointer->read_type(ptr, TYPE_STRING_POINTER)) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    if (offset < nbytes) {
        return 0;
    }
    struct string_reference* ref = pointer->read(ptr);
    ref->offset -= nbytes;
    return (u64)(0 - 1);
}

static u64 string_move_right(u64 src, u64 nbytes) {
    struct pointer* ptr = virtual->read(src);
    if (ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    if (!pointer->read_type(ptr, TYPE_STRING_POINTER)) {
        return 0;
    }
    char* data = string_pointer_internal(ptr, &size, &offset);
    if (data == 0) {
        return 0;
    }
    if (offset + nbytes >= size) {
        return 0;
    }
    struct string_reference* ref = pointer->read(ptr);
    ref->offset += nbytes;
    return (u64)(0 - 1);
}

static u64 string_strcmp(u64 src, u64 dest) {
    if (src == dest) {
        return 0;
    }
    struct pointer* src_ptr = virtual->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    struct pointer* dest_ptr = virtual->read(dest);
    if (dest_ptr == 0) {
        return 0;
    }
    u64 dest_size = 0;
    u64 dest_offset = 0;
    char* dest_data = string_pointer_internal(dest_ptr, &dest_size, &dest_offset);
    if (dest_data == 0) {
        return 0;
    }
    if (strcmp(src_data, dest_data) != 0) {
        return 0;
    }
    return (u64)(0 - 1);
}

static const struct vm_type type_definition = {
    .free = string_vm_free
};

static void INIT init(void) {
    pointer->register_type(id, type);
}

/* public */
const struct string_methods string_methods_definition = {
    .free = string_free,
    .copy = string_copy,
    .strcpy = string_strcpy,
    .strcat = string_strcat,
    .strrchr = string_strrchr,
    .strchr = string_strchr,
    .offset = string_offset,
    .match = string_match,
    .load = string_load,
    .put_char = string_put_char,
    .unsafe = string_unsafe,
    .size = string_size,
    .lessthan = string_lessthan,
    .greaterthan = string_greaterthan,
    .equals = string_equals,
    .compare = string_compare,
    .left = string_left,
    .strncpy = string_strncpy,
    .left_strncpy = string_left_strncpy,
    .right = string_right,
    .move_left = string_move_left,
    .move_right = string_move_right,
    .strcmp = string_strcmp

};

#ifndef ATTRIBUTE
void string_init(void) {
    init();
}
#endif