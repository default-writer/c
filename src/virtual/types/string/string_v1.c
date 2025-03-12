/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 12, 2025 at 9:23:09 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "string_v1.h"

#define USING_ERROR_API

#include "system/error/error_v1.h"

#include "std/data.h"

#include "virtual/api/api_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100
#define STRING_REFERENCE_SIZE sizeof(string_reference_type)

static const enum type type_id = TYPE_STRING;

/* internal */
static const_pointer_ptr get_string_internal(const_vm_ptr vm, u64 data, u64* data_size, u64* data_offset);
static const_pointer_ptr get_string_pointer_internal(const_vm_ptr vm, u64 data, u64* data_size, u64* data_offset);
static char* string_strrchr_internal(char* ch, const char* str2, u64 size, u64 offset);
static char* string_strchr_internal(char* ch, const char* str2, u64 size, u64 offset);
static const_pointer_ptr read_string_data_internal(const_vm_ptr vm, u64 str_ptr, u64* size, u64* offset);
static u64 create_string_reference_internal(const_vm_ptr vm, u64 src, u64 offset);
static char* get_last_match_internal(char* str1, const char* str2, u64* size, u64 offset);
static char* find_match_internal(char* str1, const char* str2, u64* size, u64 match_size);

/* definition */
static u64 string_free(const_vm_ptr vm, u64 ptr);
static u64 string_copy(const_vm_ptr vm, u64 ptr);
static u64 string_strcpy(const_vm_ptr vm, u64 dest_ptr, u64 src_ptr);
static u64 string_strcat(const_vm_ptr vm, u64 dest_ptr, u64 src_ptr);
static u64 string_strrchr(const_vm_ptr vm, u64 src_ptr, u64 match_ptr);
static u64 string_strchr(const_vm_ptr vm, u64 src_ptr, u64 match_ptr);
static u64 string_match(const_vm_ptr vm, u64 src_ptr, u64 match_ptr);
static u64 string_offset(const_vm_ptr vm, u64 src_ptr, u64 match_ptr);
static u64 string_load(const_vm_ptr vm, const char* ch);
static u64 string_put_char(const_vm_ptr vm, u64 ptr, char value);
static char* string_unsafe(const_vm_ptr vm, u64 ptr);
static u64 string_size(const_vm_ptr vm, u64 ptr);
static u64 string_lessthan(const_vm_ptr vm, u64 src, u64 dst);
static u64 string_greaterthan(const_vm_ptr vm, u64 src, u64 dst);
static u64 string_equals(const_vm_ptr vm, u64 src, u64 dst);
static u64 string_compare(const_vm_ptr vm, u64 src, u64 dst);
static u64 string_left(const_vm_ptr vm, u64 src, u64 shift);
static u64 string_strncpy(const_vm_ptr vm, u64 src, u64 nbytes);
static u64 string_left_strncpy(const_vm_ptr vm, u64 src, u64 nbytes);
static u64 string_right(const_vm_ptr vm, u64 src, u64 shift);
static u64 string_move_left(const_vm_ptr vm, u64 src, u64 shift);
static u64 string_move_right(const_vm_ptr vm, u64 src, u64 shift);
static u64 string_strcmp(const_vm_ptr vm, u64 src, u64 dest);

/* definition */
struct list_handler {
    stack_ptr list;
};

typedef struct string_reference* string_reference_ptr;
typedef const struct string_reference* const_string_reference_ptr;
typedef struct string_reference {
    u64 address;
    u64 offset;
} string_reference_type;

/* destructor */
static void type_desctructor(const_pointer_ptr const_ptr);

/* implementation */
static struct type_methods_definitions string_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &string_type;
    CALL(pointer)->register_known_type(type_id, safe_ptr.ptr);
}

static void type_desctructor(const_pointer_ptr const_ptr) {
    CALL(pointer)->release(const_ptr);
}

/* implementation */
static const_pointer_ptr get_string_internal(const_vm_ptr vm, u64 data, u64* data_size, u64* data_offset) {
    const_pointer_ptr data_ptr = CALL(virtual)->read(vm, data);
    u64 offset = 0;
    *data_size = CALL(pointer)->size(data_ptr);
    *data_offset = offset;
    return data_ptr;
}

static const_pointer_ptr get_string_pointer_internal(const_vm_ptr vm, u64 data, u64* data_size, u64* data_offset) {
    const_pointer_ptr data_ptr = CALL(virtual)->read(vm, data);
    u64 offset = 0;
    const_pointer_ptr const_ptr;
    const_string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    while ((const_ptr = CALL(virtual)->read(vm, ref->address)) != 0) {
        u64 ref_type_id = CALL(pointer)->get_type(const_ptr);
        if (ref_type_id == TYPE_STRING_POINTER) {
            offset += ref->offset;
            ref = CALL(pointer)->data(const_ptr);
            continue;
        }
        break;
    }
    const_ptr = CALL(virtual)->read_type(vm, ref->address, TYPE_STRING);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return NULL_PTR;
    }
    offset += ref->offset;
    *data_size = CALL(pointer)->size(const_ptr);
    *data_offset = offset;
    return const_ptr;
}

static char* string_strrchr_internal(char* ch, const char* str2, u64 size, u64 offset) {
    char* str1 = ch;
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
        return NULL_PTR;
    }
    str1 = last;
    return str1;
}

static char* string_strchr_internal(char* ch, const char* str2, u64 size, u64 offset) {
    char* str1 = ch;
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
        return NULL_PTR;
    }
    str1 = last;
    return str1;
}

/* api */
static u64 string_free(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read(vm, ptr);
    if (data_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(data_ptr == 0);
        return FALSE;
    }
    u64 data_type_id = CALL(pointer)->get_type(data_ptr);
    if (data_type_id == TYPE_STRING) {
        type_desctructor(data_ptr);
        return TRUE;
    }
    if (data_type_id == TYPE_STRING_POINTER) {
        const_string_reference_ptr ref = CALL(pointer)->data(data_ptr);
        const_pointer_ptr p_ptr = CALL(virtual)->read(vm, ref->address);
        if (p_ptr == 0) {
            ERROR_POINTER_NOT_INITIALIZED(p_ptr == 0);
            return FALSE;
        }
        type_desctructor(p_ptr);
        type_desctructor(data_ptr);
        return TRUE;
    }
    return FALSE;
}

static u64 string_copy(const_vm_ptr vm, u64 src) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    const char* ch = CALL(pointer)->data(const_ptr);
    ch += offset;
    return CALL(virtual)->memcpy(vm, size - offset, ch, type_id);
}

static u64 string_strcpy(const_vm_ptr vm, u64 dest, u64 src) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dest) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dest);
        return FALSE;
    }
    const_pointer_ptr dest_ptr = CALL(virtual)->read_type(vm, dest, type_id);
    if (dest_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dest_ptr == 0);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &src_size, &src_offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    const char* ch = CALL(pointer)->data(const_ptr);
    ch += src_offset;
    u64 dest_size = CALL(pointer)->size(dest_ptr);
    if (dest_size < src_size) {
        CALL(pointer)->realloc(dest_ptr, src_size);
    }
    u64 size = src_size - src_offset;
    char* data = CALL(pointer)->data_guard(dest_ptr, size - 1);
    virtual_api->strncpy(data, ch, size); /* NOLINT: strncpy is safe here because the destination buffer is guaranteed to be large enough */
    return TRUE;
}

static u64 string_strcat(const_vm_ptr vm, u64 dest, u64 src) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dest) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dest);
        return FALSE;
    }
    const_pointer_ptr dest_ptr = CALL(virtual)->read_type(vm, dest, type_id);
    if (dest_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dest_ptr == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    const char* ch = CALL(pointer)->data(const_ptr);
    const char* data_src = ch + offset; /* NOLINT: memory bounds check */
    u64 dest_size = CALL(pointer)->size(dest_ptr);
    size += dest_size - 1;
    if (dest_size < size) {
        CALL(pointer)->realloc(dest_ptr, size);
    }
    char* data = CALL(pointer)->data_guard(dest_ptr, size - 1);
    virtual_api->strncat(data, data_src, size - dest_size); /* NOLINT: strncat is safe here because the destination buffer is guaranteed to be large enough */
    return TRUE;
}

static u64 string_strrchr(const_vm_ptr vm, u64 src, u64 match) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    char* ch = CALL(pointer)->data(const_ptr);
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(vm, match, type_id);
    if (match_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    char* text = ch + offset;
    const char* str2 = CALL(pointer)->data(match_ptr);
    const char* str1 = string_strrchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    ch += offset;
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->address = src;
    ref->offset = offset;
    return CALL(virtual)->alloc(vm, data_ptr);
}

static u64 string_strchr(const_vm_ptr vm, u64 src, u64 match) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    char* ch = CALL(pointer)->data(const_ptr);
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(vm, match, type_id);
    if (match_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    char* text = ch + offset;
    const char* str2 = CALL(pointer)->data(match_ptr);
    const char* str1 = string_strchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    ch += offset;
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->address = src;
    ref->offset = offset;
    return CALL(virtual)->alloc(vm, data_ptr);
}

static u64 string_match(const_vm_ptr vm, u64 src, u64 match) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    char* ch = CALL(pointer)->data(const_ptr);
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(vm, match, type_id);
    if (match_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    ch += offset;
    u64 match_size = CALL(pointer)->size(match_ptr);
    const char* str2 = CALL(pointer)->data(match_ptr);
    char* str1 = ch;
    size -= offset;
    const char* found_match = find_match_internal(str1, str2, &size, match_size);
    if (found_match == 0) {
        ERROR_POINTER_NOT_INITIALIZED(found_match == 0);
        return FALSE;
    }
    offset = (u64)(found_match - ch);
    return create_string_reference_internal(vm, src, offset);
}

static char* find_match_internal(char* str1, const char* str2, u64* size, u64 match_size) {
    const char* ptr2 = str2;
    while (*size > 0) {
        while (*size > 0 && *str1 != 0 && *str1 != *str2) {
            str1++;
            (*size)--;
        }
        if (*str1 == 0 || *size < match_size) {
            return NULL_PTR;
        }
        char* ptr1 = str1;
        while (*str1 != 0 && *str2 != 0) {
            if (*str1 != *str2) {
                str1 = ++ptr1;
                (*size)--;
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
    return str1;
}

static u64 create_string_reference_internal(const_vm_ptr vm, u64 src, u64 offset) {
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->address = src;
    ref->offset = offset;
    return CALL(virtual)->alloc(vm, data_ptr);
}

static char* get_last_match_internal(char* str1, const char* str2, u64* size, u64 offset) {
    char* pos = str1;
    char* last_match = 0;
    u64 orig_size = *size - offset;
    while (orig_size > 0 && *pos != 0) {
        if (*pos == *str2) {
            last_match = pos;
        }
        ++pos;
    }
    *size = orig_size;
    return last_match;
}

static const_pointer_ptr read_string_data_internal(const_vm_ptr vm, u64 src, u64* size, u64* offset) {
    const_pointer_ptr src_ptr = CALL(virtual)->read(vm, src);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return NULL_PTR;
    }
    u64 src_type_id = CALL(pointer)->get_type(src_ptr);
    if (src_type_id == TYPE_STRING) {
        return get_string_internal(vm, src, size, offset);
    }
    if (src_type_id == TYPE_STRING_POINTER) {
        return get_string_pointer_internal(vm, src, size, offset);
    }
    return NULL_PTR;
}

static u64 string_offset(const_vm_ptr vm, u64 src, u64 match) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    char* ch = CALL(pointer)->data(const_ptr);
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(vm, match, type_id);
    if (match_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    ch += offset;
    u64 match_size = CALL(pointer)->size(match_ptr);
    char* str2 = CALL(pointer)->data(match_ptr);
    char* str1 = get_last_match_internal(ch, str2, &size, offset);
    if (str1 == 0) {
        ERROR_POINTER_NOT_INITIALIZED(match_ptr == 0);
        return FALSE;
    }
    offset = (u64)(str1 - ch);
    char* ptr2 = str2;
    size -= offset;
    while (size > 0) {
        while (size > 0 && *str1 != 0 && *str1 != *str2) {
            str1++;
            size--;
        }
        if (*str1 == 0) {
            ERROR_POINTER_NOT_INITIALIZED(*str1 == 0);
            return FALSE;
        }
        if (size < match_size) {
            ERROR_POINTER_NOT_INITIALIZED(size < match_size);
            return FALSE;
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
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->address = src;
    ref->offset = offset;
    return CALL(virtual)->alloc(vm, data_ptr);
}

static u64 string_load(const_vm_ptr vm, const char* ch) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (ch == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(ch == 0);
        return FALSE;
    }
    if (*ch == 0) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(*ch == 0);
        return FALSE;
    }
    u64 size = virtual_api->strlen(ch) + 1;
    return CALL(pointer)->alloc_guard(ch, size, size - 1, type_id);
}

static u64 string_put_char(const_vm_ptr vm, u64 src, char value) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    char* ch = CALL(pointer)->data(const_ptr);
    ch += offset;
    *ch = value;
    return TRUE;
}

static char* string_unsafe(const_vm_ptr vm, u64 src) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return NULL_PTR;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return NULL_PTR;
    }
    char* ch = CALL(pointer)->data(const_ptr);
    ch += offset;
    return ch;
}

static u64 string_size(const_vm_ptr vm, u64 src) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    return size - 1;
}

static u64 string_lessthan(const_vm_ptr vm, u64 src, u64 dst) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dst) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(vm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return FALSE;
    }
    const char* src_data = CALL(pointer)->data(src_ptr);
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(vm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dst_ptr == 0);
        return FALSE;
    }
    const char* dst_data = CALL(pointer)->data(dst_ptr);
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset >= dst_offset) {
        return FALSE;
    }
    return dst_offset - src_offset;
}

static u64 string_greaterthan(const_vm_ptr vm, u64 src, u64 dst) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dst) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(vm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return FALSE;
    }
    const char* src_data = CALL(pointer)->data(src_ptr);
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(vm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dst_ptr == 0);
        return FALSE;
    }
    const char* dst_data = CALL(pointer)->data(dst_ptr);
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset <= dst_offset) {
        return FALSE;
    }
    return src_offset - dst_offset;
}

static u64 string_equals(const_vm_ptr vm, u64 src, u64 dst) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dst) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(vm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return FALSE;
    }
    const char* src_data = CALL(pointer)->data(src_ptr);
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(vm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dst_ptr == 0);
        return FALSE;
    }
    const char* dst_data = CALL(pointer)->data(dst_ptr);
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset != dst_offset) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_compare(const_vm_ptr vm, u64 src, u64 dst) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dst) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(vm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_pte == 0);
        return FALSE;
    }
    const char* src_data = CALL(pointer)->data(src_ptr);
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(vm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dst_ptr == 0);
        return FALSE;
    }
    const char* dst_data = CALL(pointer)->data(dst_ptr);
    if (dst_data != src_data) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_left(const_vm_ptr vm, u64 src, u64 shift) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    if (offset < shift) {
        ERROR_INVALID_CONDITION(offset < shift);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->address = src;
    ref->offset = 0 - shift;
    return CALL(virtual)->alloc(vm, data_ptr);
}

static u64 string_strncpy(const_vm_ptr vm, u64 src, u64 nbytes) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    const char* ch = CALL(pointer)->data(const_ptr);
    if (offset + nbytes >= size) {
        ERROR_INVALID_CONDITION(offset + nbytes >= size);
        return FALSE;
    }
    ch += offset;
    size = nbytes + 1;
    return CALL(pointer)->alloc_guard(ch, size, size - 1, type_id);
}

static u64 string_left_strncpy(const_vm_ptr vm, u64 src, u64 nbytes) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    const char* ch = CALL(pointer)->data(const_ptr);
    if (offset < nbytes) {
        ERROR_INVALID_CONDITION(offset < nbytes);
        return FALSE;
    }
    ch += (offset - nbytes);
    size = nbytes + 1;
    return CALL(pointer)->alloc_guard(ch, size, size - 1, type_id);
}

static u64 string_right(const_vm_ptr vm, u64 src, u64 nbytes) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    if (offset + nbytes >= size) {
        ERROR_INVALID_CONDITION(offset + nbytes >= size);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->address = src;
    ref->offset = nbytes;
    return CALL(virtual)->alloc(vm, data_ptr);
}

static u64 string_move_left(const_vm_ptr vm, u64 src, u64 nbytes) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr src_ptr = CALL(virtual)->read(vm, src);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return FALSE;
    }
    u64 src_type_id = CALL(pointer)->get_type(src_ptr);
    if (src_type_id != TYPE_STRING_POINTER) {
        ERROR_INVALID_CONDITION(src_type_id != TYPE_STRING_POINTER);
        return FALSE;
    }
    const_pointer_ptr const_ptr = get_string_pointer_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    if (offset < nbytes) {
        ERROR_INVALID_CONDITION(offset < nbytes);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read(vm, src);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->offset -= nbytes;
    return TRUE;
}

static u64 string_move_right(const_vm_ptr vm, u64 src, u64 nbytes) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr src_ptr = CALL(virtual)->read(vm, src);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return FALSE;
    }
    u64 src_type_id = CALL(pointer)->get_type(src_ptr);
    if (src_type_id != TYPE_STRING_POINTER) {
        ERROR_INVALID_CONDITION(src_type_id != TYPE_STRING_POINTER);
        return FALSE;
    }
    const_pointer_ptr const_ptr = get_string_pointer_internal(vm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(const_ptr == 0);
        return FALSE;
    }
    if (offset + nbytes >= size) {
        ERROR_INVALID_CONDITION(offset + nbytes >= size);
        return FALSE;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read(vm, src);
    string_reference_ptr ref = CALL(pointer)->data(data_ptr);
    ref->offset += nbytes;
    return TRUE;
}

static u64 string_strcmp(const_vm_ptr vm, u64 src, u64 dst) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (src == dst) {
        ERROR_ARGUMENT_VALUE_NOT_INITIALIZED(src == dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(vm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(src_ptr == 0);
        return FALSE;
    }
    const char* src_data = CALL(pointer)->data(src_ptr);
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(vm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_POINTER_NOT_INITIALIZED(dst_ptr == 0);
        return FALSE;
    }
    const char* dst_data = CALL(pointer)->data(dst_ptr);
    if (virtual_api->strcmp(src_data, dst_data) != 0) {
        return FALSE;
    }
    return TRUE;
}

CVM_EXPORT void string_init(void) {
    init();
}

/* public */
const virtual_string_methods PRIVATE_API(virtual_string_methods_definitions) = {
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

const virtual_string_methods* string = &PRIVATE_API(virtual_string_methods_definitions);

const virtual_string_methods* CALL(string) {
    return string;
}
