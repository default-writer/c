/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:13:04 PM GMT+3
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

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "std/data.h"

#include "system/memory/memory_v1.h"

#include "system/os/os_v1.h"
#include "virtual/pointer/pointer_v1.h"

#include "internal/pointer_type_v1.h"
#include "internal/string_reference_type_v1.h"

#define STRING_REFERENCE_TYPE_SIZE sizeof(string_reference_type)

/* internal */
static const_pointer_ptr get_string_pointer_internal(const_vm_ptr cvm, const_pointer_ptr ptr, u64* data_size, u64* data_offset);
static char* string_strrchr_internal(char* ch, const char* str2, u64 size, u64 offset);
static char* string_strchr_internal(char* ch, const char* str2, u64 size, u64 offset);
static const_pointer_ptr read_string_data_internal(const_vm_ptr cvm, u64 str_ptr, u64* size, u64* offset);
static u64 create_string_reference_internal(const_vm_ptr cvm, u64 address, u64 offset);
static char* get_last_match_internal(char* str1, const char* str2, u64* size, u64 offset);
static char* find_match_internal(char* str1, const char* str2, u64* size, u64 match_size);

/* public */
static u64 string_free(const_vm_ptr cvm, u64 address);
static u64 string_copy(const_vm_ptr cvm, u64 ptr);
static u64 string_strcpy(const_vm_ptr cvm, u64 dest_ptr, u64 src_ptr);
static u64 string_strcat(const_vm_ptr cvm, u64 dest_ptr, u64 src_ptr);
static u64 string_strrchr(const_vm_ptr cvm, u64 src_ptr, u64 match_ptr);
static u64 string_strchr(const_vm_ptr cvm, u64 src_ptr, u64 match_ptr);
static u64 string_match(const_vm_ptr cvm, u64 src_ptr, u64 match_ptr);
static u64 string_offset(const_vm_ptr cvm, u64 src_ptr, u64 match_ptr);
static u64 string_load(const_vm_ptr cvm, const char* ch);
static u64 string_put_char(const_vm_ptr cvm, u64 ptr, char value);
static char* string_unsafe(const_vm_ptr cvm, u64 ptr);
static u64 string_size(const_vm_ptr cvm, u64 ptr);
static u64 string_lessthan(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_greaterthan(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_equals(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_compare(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_left(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_strncpy(const_vm_ptr cvm, u64 src, u64 nbytes);
static u64 string_left_strncpy(const_vm_ptr cvm, u64 src, u64 nbytes);
static u64 string_right(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_move_left(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_move_right(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_strcmp(const_vm_ptr cvm, u64 src, u64 dest);

/* definition */
struct list_handler {
    stack_ptr list;
};

/* type */
static void string_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions string_type = {
    .type_id = TYPE_STRING,
    .destructor = string_type_destructor,
};

static void string_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, string_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)string_type.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

/* internal */
static const_pointer_ptr get_string_pointer_internal(const_vm_ptr cvm, const_pointer_ptr data_ptr, u64* data_size, u64* data_offset) {
    u64 offset = 0;
    const_pointer_ptr ref_ptr;
    const_string_reference_ptr ref = data_ptr->data;
    while ((ref_ptr = CALL(pointer)->read(cvm, ref->address, TYPE_STRING_POINTER)) != 0) {
        offset += ref->offset;
        ref = ref_ptr->data;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, ref->address, TYPE_STRING);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, ref->address, (u64)string_type.type_id);
        return NULL_PTR;
    }
    offset += ref->offset;
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    *data_size = public_ptr->size;
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

static u64 create_string_reference_internal(const_vm_ptr cvm, u64 address, u64 offset) {
    void_ptr data = CALL(memory)->alloc(STRING_REFERENCE_TYPE_SIZE);
    u64 data_ptr = CALL(pointer)->alloc(cvm, data, STRING_REFERENCE_TYPE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = data;
    ref->address = address;
    ref->offset = offset;
    return data_ptr;
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

static const_pointer_ptr read_string_data_internal(const_vm_ptr cvm, u64 address, u64* size, u64* offset) {
    const_pointer_ptr string_data_ptr = CALL(pointer)->read(cvm, address, TYPE_STRING);
    if (string_data_ptr != 0) {
        safe_pointer_ptr safe_ptr;
        safe_ptr.const_ptr = string_data_ptr;
        pointer_ptr ptr = safe_ptr.ptr;
        if (ptr != 0) {
            const_pointer_public_ptr public_ptr = &ptr->public;
            *size = public_ptr->size;
            *offset = 0;
            return string_data_ptr;
        }
    }
    const_pointer_ptr string_pointer_const_ptr = CALL(pointer)->read(cvm, address, TYPE_STRING_POINTER);
    if (string_pointer_const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)string_pointer_const_ptr, address, (u64)string_type.type_id);
        return NULL_PTR;
    }
    return get_string_pointer_internal(cvm, string_pointer_const_ptr, size, offset);
}

/* public */
static u64 string_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    string_type_destructor(cvm, address);
    return TRUE;
}

static u64 string_copy(const_vm_ptr cvm, u64 src) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const char* ch = const_ptr->data;
    ch += offset;
    u64 address = CALL(pointer)->copy(cvm, ch, size - offset, 0, string_type.type_id);
    return address;
}

static u64 string_strcpy(const_vm_ptr cvm, u64 dest, u64 src) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (dest == 0) {
        ERROR_INVALID_ARGUMENT("dest == %lld", dest);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (src == dest) {
        ERROR_INVALID_VALUE("src == %lld, dest== %lld", src, dest);
        return FALSE;
    }
    const_pointer_ptr dest_ptr = CALL(pointer)->read(cvm, dest, string_type.type_id);
    if (dest_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)dest_ptr, dest, (u64)string_type.type_id);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &src_size, &src_offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, src_size == %lld, src_offset == %lld", (const_void_ptr)const_ptr, src, src_size, src_offset);
        return FALSE;
    }
    const char* ch = const_ptr->data;
    ch += src_offset;
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = dest_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 dest_size = public_ptr->size;
    if (dest_size < src_size) {
        if (ptr->data != 0) {
            ptr->data = CALL(memory)->realloc(ptr->data, dest_size, src_size);
        }
        ptr->public.size = src_size;
    }
    u64 size = src_size - src_offset;
    safe_void_ptr safe_dst_ptr;
    safe_dst_ptr.const_ptr = ptr->data;
    u8* dst_ptr = safe_dst_ptr.ptr;
    dst_ptr[size - 1] = 0;
    char* data = (char*)dst_ptr;
    CALL(os)->strncpy(data, ch, size); /* NOLINT: strncpy is safe here because the destination buffer is guaranteed to be large enough */
    return TRUE;
}

static u64 string_strcat(const_vm_ptr cvm, u64 dest, u64 src) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (dest == 0) {
        ERROR_INVALID_ARGUMENT("dest == %lld", dest);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (src == dest) {
        ERROR_INVALID_VALUE("src == %lld, dest== %lld", src, dest);
        return FALSE;
    }
    const_pointer_ptr dest_ptr = CALL(pointer)->read(cvm, dest, string_type.type_id);
    if (dest_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)dest_ptr, dest, (u64)string_type.type_id);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const char* ch = const_ptr->data;
    const char* data_src = ch + offset; /* NOLINT: memory bounds check */
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = dest_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 dest_size = public_ptr->size;
    size += dest_size - 1;
    u64 src_size = size;
    if (dest_size < src_size) {
        if (ptr->data != 0) {
            ptr->data = CALL(memory)->realloc(ptr->data, dest_size, src_size);
        }
        ptr->public.size = src_size;
    }
    safe_void_ptr safe_dst_ptr;
    safe_dst_ptr.const_ptr = ptr->data;
    u8* dst_ptr = safe_dst_ptr.ptr;
    dst_ptr[size - 1] = 0;
    char* data = (char*)dst_ptr;
    CALL(os)->strncat(data, data_src, size - dest_size); /* NOLINT: strncat is safe here because the destination buffer is guaranteed to be large enough */
    return TRUE;
}

static u64 string_strrchr(const_vm_ptr cvm, u64 src, u64 match) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (match == 0) {
        ERROR_INVALID_ARGUMENT("match == %lld", match);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    char* ch = src_data_ptr;
    char* text = ch + offset;
    const_pointer_ptr match_const_ptr = CALL(pointer)->read(cvm, match, string_type.type_id);
    if (match_const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)match_const_ptr, match, (u64)string_type.type_id);
        return FALSE;
    }
    const char* str2 = match_const_ptr->data;
    const char* str1 = string_strrchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        ERROR_INVALID_VALUE("str1 == %p, str2 == %p, size == %lld, offset == %lld", str1, str2, size, offset);
        return FALSE;
    }
    ch += offset;
    offset = (u64)(str1 - ch);
    u64 data_ptr = create_string_reference_internal(cvm, src, offset);
    return data_ptr;
}

static u64 string_strchr(const_vm_ptr cvm, u64 src, u64 match) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (match == 0) {
        ERROR_INVALID_ARGUMENT("match == %lld", match);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    char* ch = src_data_ptr;
    char* text = ch + offset;
    const_pointer_ptr match_const_ptr = CALL(pointer)->read(cvm, match, string_type.type_id);
    if (match_const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)match_const_ptr, match, (u64)string_type.type_id);
        return FALSE;
    }
    const char* str2 = match_const_ptr->data;
    const char* str1 = string_strchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        ERROR_INVALID_VALUE("str1 == %p, str2 == %p, size == %lld, offset == %lld", str1, str2, size, offset);
        return FALSE;
    }
    ch += offset;
    offset = (u64)(str1 - ch);
    u64 data_ptr = create_string_reference_internal(cvm, src, offset);
    return data_ptr;
}

static u64 string_match(const_vm_ptr cvm, u64 src, u64 match) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (match == 0) {
        ERROR_INVALID_ARGUMENT("match == %lld", match);
        return FALSE;
    }
    const_pointer_ptr match_ptr = CALL(pointer)->read(cvm, match, string_type.type_id);
    if (match_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)match_ptr, match, (u64)string_type.type_id);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    char* ch = src_data_ptr;
    ch += offset;
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = match_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    const char* str2 = match_ptr->data;
    u64 match_size = public_ptr->size;
    char* str1 = ch;
    size -= offset;
    const char* found_match = find_match_internal(str1, str2, &size, match_size);
    if (found_match == 0) {
        ERROR_INVALID_VALUE("found_match == %p, size == %lld, match_size == %lld", found_match, size, match_size);
        return FALSE;
    }
    offset = (u64)(found_match - ch);
    return create_string_reference_internal(cvm, src, offset);
}

static u64 string_offset(const_vm_ptr cvm, u64 src, u64 match) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (match == 0) {
        ERROR_INVALID_ARGUMENT("match == %lld", match);
        return FALSE;
    }
    const_pointer_ptr match_ptr = CALL(pointer)->read(cvm, match, string_type.type_id);
    if (match_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)match_ptr, match, (u64)string_type.type_id);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    char* ch = src_data_ptr;
    ch += offset;
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = match_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    const_pointer_public_ptr public_ptr = &ptr->public;
    u64 match_size = public_ptr->size;
    const_void_ptr match_const_data_ptr = match_ptr->data;
    safe_void_ptr match_void_ptr;
    match_void_ptr.const_ptr = match_const_data_ptr;
    void_ptr match_data_ptr = match_void_ptr.ptr;
    char* str2 = match_data_ptr;
    char* str1 = get_last_match_internal(ch, str2, &size, offset);
    if (str1 == 0) {
        ERROR_INVALID_VALUE("str1 == %p, str2 == %p, size == %lld, offset == %lld", str1, str2, size, offset);
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
            ERROR_INVALID_VALUE("str1 == %p, size == %lld, offset == %lld", str1, size, offset);
            return FALSE;
        }
        if (size < match_size) {
            ERROR_INVALID_VALUE("size == %lld, match_size == %lld", size, match_size);
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
    u64 data_ptr = create_string_reference_internal(cvm, src, offset);
    return data_ptr;
}

static u64 string_load(const_vm_ptr cvm, const char* ch) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (ch == 0) {
        ERROR_INVALID_ARGUMENT("ch == %p", ch);
        return FALSE;
    }
    if (*ch == 0) {
        ERROR_INVALID_ARGUMENT("*ch == %lld", (u64)*ch);
        return FALSE;
    }
    u64 size = CALL(os)->strlen(ch) + 1;
    u64 address = CALL(pointer)->copy(cvm, ch, size, size - 1, string_type.type_id);
    return address;
}

static u64 string_put_char(const_vm_ptr cvm, u64 src, char value) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    char* ch = src_data_ptr;
    ch += offset;
    *ch = value;
    return TRUE;
}

static char* string_unsafe(const_vm_ptr cvm, u64 src) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, asddress = %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return NULL_PTR;
    }
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    char* ch = src_data_ptr;
    ch += offset;
    return ch;
}

static u64 string_size(const_vm_ptr cvm, u64 src) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address ==- %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    return size - 1;
}

static u64 string_lessthan(const_vm_ptr cvm, u64 src, u64 dst) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (dst == 0) {
        ERROR_INVALID_ARGUMENT("dst == %lld", dst);
        return FALSE;
    }
    if (src == dst) {
        ERROR_INVALID_VALUE("src == %lld, dst== %lld", src, dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(cvm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, src_size == %lld, src_offset == %lld", (const_void_ptr)src_ptr, src, src_size, src_offset);
        return FALSE;
    }
    const char* src_data = src_ptr->data;
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(cvm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, dst_size == %lld, dst_offset == %lld", (const_void_ptr)dst_ptr, dst, dst_size, dst_offset);
        return FALSE;
    }
    const char* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset >= dst_offset) {
        return FALSE;
    }
    return dst_offset - src_offset;
}

static u64 string_greaterthan(const_vm_ptr cvm, u64 src, u64 dst) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (dst == 0) {
        ERROR_INVALID_ARGUMENT("dst == %lld", dst);
        return FALSE;
    }
    if (src == dst) {
        ERROR_INVALID_VALUE("src == %lld, dst== %lld", src, dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(cvm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)src_ptr, src, src_size, src_offset);
        return FALSE;
    }
    const char* src_data = src_ptr->data;
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(cvm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)dst_ptr, dst, dst_size, dst_offset);
        return FALSE;
    }
    const char* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset <= dst_offset) {
        return FALSE;
    }
    return src_offset - dst_offset;
}

static u64 string_equals(const_vm_ptr cvm, u64 src, u64 dst) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (dst == 0) {
        ERROR_INVALID_ARGUMENT("dst == %lld", dst);
        return FALSE;
    }
    if (src == dst) {
        ERROR_INVALID_VALUE("src == %lld, dst== %lld", src, dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(cvm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)src_ptr, src, src_size, src_offset);
        return FALSE;
    }
    const char* src_data = src_ptr->data;
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(cvm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)dst_ptr, dst, dst_size, dst_offset);
        return FALSE;
    }
    const char* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset != dst_offset) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_compare(const_vm_ptr cvm, u64 src, u64 dst) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (dst == 0) {
        ERROR_INVALID_ARGUMENT("dst == %lld", dst);
        return FALSE;
    }
    if (src == dst) {
        ERROR_INVALID_VALUE("src == %lld, dst== %lld", src, dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(cvm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)src_ptr, src, src_size, src_offset);
        return FALSE;
    }
    const char* src_data = src_ptr->data;
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(cvm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)dst_ptr, dst, dst_size, dst_offset);
        return FALSE;
    }
    const char* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_left(const_vm_ptr cvm, u64 src, u64 shift) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (shift == 0) {
        ERROR_INVALID_ARGUMENT("shift == %lld", shift);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    if (offset < shift) {
        ERROR_INVALID_VALUE("offset == %lld, shift == %lld", offset, shift);
        return FALSE;
    }
    u64 ptr = create_string_reference_internal(cvm, src, 0 - shift);
    return ptr;
}

static u64 string_strncpy(const_vm_ptr cvm, u64 src, u64 nbytes) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (nbytes == 0) {
        ERROR_INVALID_ARGUMENT("nbytes == %lld", nbytes);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const char* ch = const_ptr->data;
    if (offset + nbytes >= size) {
        ERROR_INVALID_VALUE("offset == %lld, nbytes == %lld, size== %lld", offset, nbytes, size);
        return FALSE;
    }
    ch += offset;
    size = nbytes + 1;
    u64 address = CALL(pointer)->copy(cvm, ch, size, size - 1, string_type.type_id);
    return address;
}

static u64 string_left_strncpy(const_vm_ptr cvm, u64 src, u64 nbytes) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (nbytes == 0) {
        ERROR_INVALID_ARGUMENT("nbytes == %lld", nbytes);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    const char* ch = const_ptr->data;
    if (offset < nbytes) {
        ERROR_INVALID_VALUE("offset == %lld, nbytes == %lld", offset, nbytes);
        return FALSE;
    }
    ch += (offset - nbytes);
    size = nbytes + 1;
    u64 address = CALL(pointer)->copy(cvm, ch, size, size - 1, string_type.type_id);
    return address;
}

static u64 string_right(const_vm_ptr cvm, u64 src, u64 nbytes) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (nbytes == 0) {
        ERROR_INVALID_ARGUMENT("nbytes == %lld", nbytes);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = read_string_data_internal(cvm, src, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, src, size, offset);
        return FALSE;
    }
    if (offset + nbytes >= size) {
        ERROR_INVALID_VALUE("offset == %lld, nbytes == %lld, size== %lld", offset, nbytes, size);
        return FALSE;
    }
    void_ptr data;
    u64 data_ptr = create_string_reference_internal(cvm, src, nbytes);
    return data_ptr;
}

static u64 string_move_left(const_vm_ptr cvm, u64 address, u64 nbytes) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    if (nbytes == 0) {
        ERROR_INVALID_ARGUMENT("nbytes == %lld", nbytes);
        return FALSE;
    }
    const_pointer_ptr src_ptr = CALL(pointer)->read(cvm, address, TYPE_STRING_POINTER);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)src_ptr, address, (u64)TYPE_STRING_POINTER);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr const_ptr = get_string_pointer_internal(cvm, src_ptr, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, address, size, offset);
        return FALSE;
    }
    if (offset < nbytes) {
        ERROR_INVALID_VALUE("offset == %lld, nbytes == %lld", offset, nbytes);
        return FALSE;
    }
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_ptr->data;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    string_reference_ptr ref = src_data_ptr;
    ref->offset -= nbytes;
    return TRUE;
}

static u64 string_move_right(const_vm_ptr cvm, u64 address, u64 nbytes) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    if (nbytes == 0) {
        ERROR_INVALID_ARGUMENT("nbytes == %lld", nbytes);
        return FALSE;
    }
    u64 size = 0;
    u64 offset = 0;
    const_pointer_ptr src_ptr = CALL(pointer)->read(cvm, address, TYPE_STRING_POINTER);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)src_ptr, address, (u64)TYPE_STRING_POINTER);
        return FALSE;
    }
    const_pointer_ptr const_ptr = get_string_pointer_internal(cvm, src_ptr, &size, &offset);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)const_ptr, address, size, offset);
        return FALSE;
    }
    if (offset + nbytes >= size) {
        ERROR_INVALID_VALUE("offset == %lld, nbytes == %lld, size== %lld", offset, nbytes, size);
        return FALSE;
    }
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_ptr->data;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    string_reference_ptr ref = src_data_ptr;
    ref->offset += nbytes;
    return TRUE;
}

static u64 string_strcmp(const_vm_ptr cvm, u64 src, u64 dst) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (src == 0) {
        ERROR_INVALID_ARGUMENT("src == %lld", src);
        return FALSE;
    }
    if (dst == 0) {
        ERROR_INVALID_ARGUMENT("dst == %lld", dst);
        return FALSE;
    }
    if (src == dst) {
        ERROR_INVALID_VALUE("src == %lld, dst== %lld", src, dst);
        return FALSE;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_string_data_internal(cvm, src, &src_size, &src_offset);
    if (src_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)src_ptr, src, src_size, src_offset);
        return FALSE;
    }
    const char* src_data = src_ptr->data;
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const_pointer_ptr dst_ptr = read_string_data_internal(cvm, dst, &dst_size, &dst_offset);
    if (dst_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, size == %lld, offset == %lld", (const_void_ptr)dst_ptr, dst, dst_size, dst_offset);
        return FALSE;
    }
    const char* dst_data = dst_ptr->data;
    if (CALL(os)->strcmp(src_data, dst_data) != 0) {
        return FALSE;
    }
    return TRUE;
}

/* public */
CVM_EXPORT void string_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &string_type;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

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

const virtual_string_methods* PRIVATE_API(string) = &PRIVATE_API(virtual_string_methods_definitions);
const virtual_string_methods* CALL(string) {
    return PRIVATE_API(string);
}
