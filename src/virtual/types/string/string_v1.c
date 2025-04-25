/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 25, 2025 at 4:46:14 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "string_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "std/data.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/list/list_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include "internal/pointer_type_v1.h"

/* internal */
static u64 create_string_reference_internal(const_vm_ptr cvm, const_pointer_ptr const_ptr, u64 offset);
static u8* string_strrchr_internal(u8* ch, const u8* str2, u64 size, u64 offset);
static u8* string_strchr_internal(u8* str1, u8 ch, u64 size, u64 offset);
static u8* get_next_match_internal(u8* str1, u8* str2, u64 size, u64 offset, u64 match_size);
static u8* get_match_offset_internal(u8* str1, const u8* str2, u64 size, u64 offset);
static const_pointer_ptr read_internal(const_vm_ptr cvm, u64 address);
static const_pointer_ptr read_offset_internal(const_vm_ptr cvm, u64 address, u64* offset);

/* public */
static u64 string_free(const_vm_ptr cvm, u64 address);
static u64 string_copy(const_vm_ptr cvm, u64 ptr);
static u64 string_strncpy(const_vm_ptr cvm, u64 dest, u64 src);
static u64 string_strcat(const_vm_ptr cvm, u64 dest, u64 src);
static u64 string_strrchr(const_vm_ptr cvm, u64 src, u64 match);
static u64 string_strchr(const_vm_ptr cvm, u64 src, u64 match);
static u64 string_match(const_vm_ptr cvm, u64 src, u64 match);
static u64 string_match_offset(const_vm_ptr cvm, u64 src, u64 match);
static u64 string_load(const_vm_ptr cvm, const char* ch);
static u64 string_put_char(const_vm_ptr cvm, u64 ptr, u8 value);
static char* string_unsafe(const_vm_ptr cvm, u64 ptr);
static u64 string_size(const_vm_ptr cvm, u64 ptr);
static u64 string_lessthan(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_greaterthan(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_equals(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_compare(const_vm_ptr cvm, u64 src, u64 dst);
static u64 string_left(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_right(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_left_copy(const_vm_ptr cvm, u64 src, u64 nbytes);
static u64 string_right_copy(const_vm_ptr cvm, u64 src, u64 nbytes);
static u64 string_move_left(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_move_right(const_vm_ptr cvm, u64 src, u64 shift);
static u64 string_strcmp(const_vm_ptr cvm, u64 src, u64 dest);
static u64 string_ref(const_vm_ptr cvm, u64 src, u64 depth);

/* definition */
struct list_handler {
    stack_ptr list;
};

/* type */
static void string_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions string_type_definitions = {
    .type_id = TYPE_STRING,
    .destructor = string_type_destructor,
};

static void string_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, string_type_definitions.type_id);
    CHECK_POINTER_VOID(const_ptr);
    CALL(pointer)->free(cvm, address);
}

/* internal */
static u64 create_string_reference_internal(const_vm_ptr cvm, const_pointer_ptr const_ptr, u64 offset) {
    u64 size = const_ptr->public.size;
    void_ptr data = (void_ptr)const_ptr->public.address;
    u64 address = CALL(pointer)->alloc(cvm, data, size, offset, FLAG_MEMORY_ADDRESS, string_type_definitions.type_id);
    return address;
}

static u8* string_strrchr_internal(u8* ch, const u8* str2, u64 size, u64 offset) {
    u8* str1 = ch;
    u8* ptr = str1;
    u8* last = 0;
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

static u8* string_strchr_internal(u8* str1, u8 ch, u64 size, u64 offset) {
    u8* ptr = str1;
    u8* last = 0;
    u64 rsize = size - 1;
    while (rsize > offset && *ptr != 0 && last == 0) {
        if (*ptr == ch) {
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

static u8* get_next_match_internal(u8* str1, u8* str2, u64 size, u64 offset, u64 match_size) {
    u8* ptr2 = str2;
    u64 i = size - offset;
    while (i > 0) {
        while (i > 0 && *str1 != 0 && *str1 != *str2) {
            str1++;
            i--;
        }
        if (*str1 == 0 || i < match_size)
            return NULL_PTR;
        u8* ptr1 = str1;
        while (*str1 != 0 && *str2 != 0) {
            if (*str1 != *str2) {
                str1 = ++ptr1;
                i--;
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

static u8* get_match_offset_internal(u8* str1, const u8* str2, u64 size, u64 offset) {
    u8* pos = str1;
    u8* last_match = 0;
    u64 i = size - offset;
    while (i-- > 1 && *pos != 0) {
        if (*pos == *str2) {
            last_match = pos;
        }
        ++pos;
    }
    return last_match;
}

static const_pointer_ptr read_internal(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, string_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_PTR);
    while (IS_FLAG_MEMORY_ADDRESS(const_ptr->flags)) {
        const_ptr = CALL(pointer)->read(cvm, (u64)const_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(const_ptr, NULL_PTR);
        CHECK_VALUE(const_ptr->data, NULL_PTR);
    }
    return const_ptr;
}

static const_pointer_ptr read_offset_internal(const_vm_ptr cvm, u64 address, u64* offset) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, string_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_PTR);
    *offset = const_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(const_ptr->flags)) {
        const_ptr = CALL(pointer)->read(cvm, (u64)const_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(const_ptr, NULL_PTR);
        CHECK_VALUE(const_ptr->data, NULL_PTR);
        *offset += const_ptr->offset;
    }
    return const_ptr;
}

/* public */
static u64 string_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    string_type_destructor(cvm, address);
    return TRUE;
}

u64 string_split(const_vm_ptr cvm, u64 src, stack_ptr stack) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_ARG(stack, FALSE);
    const_pointer_ptr const_ptr = read_internal(cvm, src);
    CHECK_POINTER(const_ptr, FALSE);
    CHECK_VALUE(const_ptr->data, FALSE);
    u64 text_size = const_ptr->public.size;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    char* text = src_void_ptr.ptr;
    char* tmp = text;
    while (text_size > 0 && text != 0 && *tmp != 0) {
        while (text_size-- > 0 && *tmp != 0 && *tmp != '\n') {
            tmp++;
        }
        if (text_size == 0) {
            return FALSE;
        }
        *tmp++ = '\0';
        u64 string_ptr = CALL(string)->load(cvm, text);
        CALL(list)->push(cvm, stack, (void_ptr)string_ptr);
        text = tmp;
    }
    return TRUE;
}

static u64 string_copy(const_vm_ptr cvm, u64 src) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 size = const_ptr->public.size;
    u64 offset = const_offset;
    const u8* ch = const_ptr->data;
    ch += offset;
    u64 address = CALL(pointer)->copy(cvm, ch, size - offset, 0, string_type_definitions.type_id);
    return address;
}

static u64 string_strncpy(const_vm_ptr cvm, u64 dest, u64 src) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(dest, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_CONDITION(src == dest, FALSE);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, dest, &const_offset);
    CHECK_POINTER(const_ptr, FALSE);
    CHECK_VALUE(const_ptr->data, FALSE);
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_offset_internal(cvm, src, &src_offset);
    CHECK_POINTER(src_ptr, FALSE);
    CHECK_VALUE(src_ptr->data, FALSE);
    u64 src_size = src_ptr->public.size;
    const u8* byte = src_ptr->data;
    byte += src_offset;
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    u64 dest_size = const_ptr->public.size;
    if (dest_size < src_size) {
        if (IS_FLAG_MEMORY_PTR(ptr->flags) && ptr->data != 0) {
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
    CALL(os)->strncpy(data, (const char*)byte, size); /* NOLINT: strncpy is safe here because the destination buffer is guaranteed to be large enough */
    return TRUE;
}

static u64 string_strcat(const_vm_ptr cvm, u64 dest, u64 src) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(dest, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_CONDITION(src == dest, FALSE);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, dest, &const_offset);
    CHECK_POINTER(const_ptr, FALSE);
    CHECK_VALUE(const_ptr->data, FALSE);
    u64 src_offset = 0;
    const_pointer_ptr src_ptr = read_offset_internal(cvm, src, &src_offset);
    CHECK_POINTER(src_ptr, FALSE);
    CHECK_VALUE(src_ptr->data, FALSE);
    u64 size = src_ptr->public.size;
    const u8* ch = src_ptr->data;
    CHECK_VALUE(src_ptr->data, FALSE);
    const u8* data_src = ch + src_offset; /* NOLINT: memory bounds check */
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    u64 dest_size = const_ptr->public.size;
    size += dest_size - src_offset - 1;
    u64 src_size = size;
    if (dest_size < src_size) {
        if (IS_FLAG_MEMORY_PTR(ptr->flags) && ptr->data != 0) {
            ptr->data = CALL(memory)->realloc(ptr->data, dest_size, src_size);
        }
        ptr->public.size = src_size;
    }
    safe_void_ptr safe_dst_ptr;
    safe_dst_ptr.const_ptr = ptr->data;
    u8* dst_ptr = safe_dst_ptr.ptr;
    dst_ptr[size - 1] = 0;
    char* data = (char*)dst_ptr;
    CALL(os)->strncat(data, (const char*)data_src, size - dest_size); /* NOLINT: strncat is safe here because the destination buffer is guaranteed to be large enough */
    return TRUE;
}

static u64 string_strrchr(const_vm_ptr cvm, u64 src, u64 match) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(match, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 match_offset = 0;
    const_pointer_ptr match_ptr = read_offset_internal(cvm, match, &match_offset);
    CHECK_POINTER(match_ptr, NULL_ADDRESS);
    CHECK_VALUE(match_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    u8* ch = src_data_ptr;
    u8* text = ch + offset;
    const u8* str2 = (const u8*)match_ptr->data + match_offset;
    const u8* str1 = (const u8*)string_strrchr_internal(text, str2, size, offset);
    CHECK_VALUE(str1, NULL_ADDRESS);
    ch += offset;
    offset = (u64)(str1 - ch);
    u64 address = create_string_reference_internal(cvm, const_ptr, offset);
    return address;
}

static u64 string_strchr(const_vm_ptr cvm, u64 src, u64 match) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(match, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 match_offset = 0;
    const_pointer_ptr match_ptr = read_offset_internal(cvm, match, &match_offset);
    CHECK_POINTER(match_ptr, NULL_ADDRESS);
    CHECK_VALUE(match_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    u8* ch = src_data_ptr;
    u8* text = ch + offset;
    const u8* str2 = (const u8*)match_ptr->data + match_offset;
    const u8* str1 = string_strchr_internal(text, *str2, size, offset);
    CHECK_VALUE(str1, NULL_ADDRESS);
    ch += offset;
    offset += (u64)(str1 - ch);
    u64 address = create_string_reference_internal(cvm, const_ptr, offset);
    return address;
}

static u64 string_match(const_vm_ptr cvm, u64 src, u64 match) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(match, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);

    u64 match_offset = 0;
    const_pointer_ptr match_ptr = read_offset_internal(cvm, match, &match_offset);
    CHECK_POINTER(match_ptr, NULL_ADDRESS);
    CHECK_VALUE(match_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size;
    u64 match_size = match_ptr->public.size;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    const u8* data = const_ptr->data;
    u8* ch = src_data_ptr;
    ch += offset;
    const_void_ptr match_const_data_ptr = match_ptr->data;
    safe_void_ptr match_void_ptr;
    match_void_ptr.const_ptr = match_const_data_ptr;
    void_ptr match_data_ptr = match_void_ptr.ptr;
    u8* str2 = match_data_ptr;
    u8* str1 = ch;
    CHECK_VALUE(str1, NULL_ADDRESS);
    CHECK_VALUE(str2, NULL_ADDRESS);
    str1 = get_next_match_internal(str1, str2, size, offset, match_size);
    CHECK_VALUE(str1, NULL_ADDRESS);
    offset = (u64)(str1 - data);
    u64 address = create_string_reference_internal(cvm, const_ptr, offset);
    return address;
}

static u64 string_match_offset(const_vm_ptr cvm, u64 src, u64 match) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(match, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 match_offset = 0;
    const_pointer_ptr match_ptr = read_offset_internal(cvm, match, &match_offset);
    CHECK_POINTER(match_ptr, NULL_ADDRESS);
    CHECK_VALUE(match_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size;
    u64 match_size = match_ptr->public.size;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    const u8* data = const_ptr->data;
    u8* ch = src_data_ptr;
    ch += offset;
    const_void_ptr match_const_data_ptr = match_ptr->data;
    safe_void_ptr match_void_ptr;
    match_void_ptr.const_ptr = match_const_data_ptr;
    void_ptr match_data_ptr = match_void_ptr.ptr;
    u8* str2 = match_data_ptr;
    u8* str1 = get_match_offset_internal(ch, str2, size, offset);
    CHECK_VALUE(str1, NULL_ADDRESS);
    CHECK_VALUE(str2, NULL_ADDRESS);
    offset += (u64)(str1 - ch);
    str1 = get_next_match_internal(str1, str2, size, offset, match_size);
    CHECK_VALUE(str1, NULL_ADDRESS);
    offset = (u64)(str1 - data);
    u64 address = create_string_reference_internal(cvm, const_ptr, offset);
    return address;
}

static u64 string_load(const_vm_ptr cvm, const char* ch) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(ch, NULL_ADDRESS);
    // CHECK_VALUE_NO_ERROR(*ch, NULL_ADDRESS);
    u64 size = *ch == 0 ? 1 : CALL(os)->strlen((const char*)ch) + 1;
    u64 address = CALL(pointer)->copy(cvm, ch, size, 0, string_type_definitions.type_id);
    return address;
}

static u64 string_put_char(const_vm_ptr cvm, u64 src, u8 value) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, FALSE);
    CHECK_VALUE(const_ptr->data, FALSE);
    u64 offset = const_offset;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    u8* ch = src_data_ptr;
    ch += offset;
    *ch = value;
    return TRUE;
}

static char* string_unsafe(const_vm_ptr cvm, u64 src) {
    CHECK_VM(cvm, NULL_PTR);
    CHECK_ARG(src, NULL_PTR);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_PTR);
    CHECK_VALUE(const_ptr->data, NULL_PTR);
    u64 offset = const_offset;
    const_void_ptr src_const_data_ptr = const_ptr->data;
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = src_const_data_ptr;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    u8* ch = src_data_ptr;
    ch += offset;
    return (char*)ch;
}

static u64 string_size(const_vm_ptr cvm, u64 src) {
    CHECK_VM(cvm, NULL_VALUE);
    CHECK_ARG(src, NULL_VALUE);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_VALUE);
    CHECK_VALUE(const_ptr->data, NULL_VALUE);
    u64 size = const_ptr->public.size;
    return size - 1;
}

static u64 string_lessthan(const_vm_ptr cvm, u64 src, u64 dst) {
    CHECK_VM(cvm, NULL_VALUE);
    CHECK_ARG(src, NULL_VALUE);
    CHECK_ARG(dst, NULL_VALUE);
    CHECK_CONDITION(src == dst, NULL_VALUE);
    const_pointer_ptr src_ptr = CALL(pointer)->read(cvm, src, string_type_definitions.type_id);
    CHECK_POINTER(src_ptr, FALSE);
    u64 src_offset = src_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(src_ptr->flags)) {
        src_ptr = CALL(pointer)->read(cvm, (u64)src_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(src_ptr, FALSE);
        CHECK_VALUE(src_ptr->data, FALSE);
        src_offset += src_ptr->offset;
    }
    CHECK_VALUE(src_ptr->data, FALSE);
    const_pointer_ptr dst_ptr = CALL(pointer)->read(cvm, dst, string_type_definitions.type_id);
    CHECK_POINTER(dst_ptr, FALSE);
    u64 dst_offset = dst_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(dst_ptr->flags)) {
        dst_ptr = CALL(pointer)->read(cvm, (u64)dst_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(dst_ptr, FALSE);
        CHECK_VALUE(dst_ptr->data, FALSE);
        dst_offset += dst_ptr->offset;
    }
    CHECK_VALUE(dst_ptr->data, FALSE);
    const u8* src_data = src_ptr->data;
    const u8* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return NULL_VALUE;
    }
    if (src_offset >= dst_offset) {
        return NULL_VALUE;
    }
    return dst_offset - src_offset;
}

static u64 string_greaterthan(const_vm_ptr cvm, u64 src, u64 dst) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_ARG(dst, FALSE);
    CHECK_CONDITION(src == dst, FALSE);
    const_pointer_ptr src_ptr = CALL(pointer)->read(cvm, src, string_type_definitions.type_id);
    CHECK_POINTER(src_ptr, FALSE);
    u64 src_offset = src_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(src_ptr->flags)) {
        src_ptr = CALL(pointer)->read(cvm, (u64)src_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(src_ptr, FALSE);
        CHECK_VALUE(src_ptr->data, FALSE);
        src_offset += src_ptr->offset;
    }
    CHECK_VALUE(src_ptr->data, FALSE);
    const_pointer_ptr dst_ptr = CALL(pointer)->read(cvm, dst, string_type_definitions.type_id);
    CHECK_POINTER(dst_ptr, FALSE);
    u64 dst_offset = dst_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(dst_ptr->flags)) {
        dst_ptr = CALL(pointer)->read(cvm, (u64)dst_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(dst_ptr, FALSE);
        CHECK_VALUE(dst_ptr->data, FALSE);
        dst_offset += dst_ptr->offset;
    }
    CHECK_VALUE(dst_ptr->data, FALSE);
    const u8* src_data = src_ptr->data;
    const u8* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return NULL_VALUE;
    }
    if (src_offset <= dst_offset) {
        return NULL_VALUE;
    }
    return src_offset - dst_offset;
}

static u64 string_equals(const_vm_ptr cvm, u64 src, u64 dst) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_ARG(dst, FALSE);
    CHECK_CONDITION(src == dst, NULL_VALUE);
    const_pointer_ptr src_ptr = CALL(pointer)->read(cvm, src, string_type_definitions.type_id);
    CHECK_POINTER(src_ptr, FALSE);
    u64 src_offset = src_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(src_ptr->flags)) {
        src_ptr = CALL(pointer)->read(cvm, (u64)src_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(src_ptr, FALSE);
        CHECK_VALUE(src_ptr->data, FALSE);
        src_offset += src_ptr->offset;
    }
    CHECK_VALUE(src_ptr->data, FALSE);
    const_pointer_ptr dst_ptr = CALL(pointer)->read(cvm, dst, string_type_definitions.type_id);
    CHECK_POINTER(dst_ptr, FALSE);
    u64 dst_offset = dst_ptr->offset;
    while (IS_FLAG_MEMORY_ADDRESS(dst_ptr->flags)) {
        dst_ptr = CALL(pointer)->read(cvm, (u64)dst_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(dst_ptr, FALSE);
        CHECK_VALUE(dst_ptr->data, FALSE);
        dst_offset += dst_ptr->offset;
    }
    CHECK_VALUE(dst_ptr->data, FALSE);
    const u8* src_data = src_ptr->data;
    const u8* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return FALSE;
    }
    if (src_offset != dst_offset) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_compare(const_vm_ptr cvm, u64 src, u64 dst) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_ARG(dst, FALSE);
    CHECK_CONDITION(src == dst, FALSE);
    const_pointer_ptr src_ptr = read_internal(cvm, src);
    CHECK_POINTER(src_ptr, FALSE);
    CHECK_VALUE(src_ptr->data, FALSE);
    const_pointer_ptr dst_ptr = read_internal(cvm, dst);
    CHECK_POINTER(dst_ptr, FALSE);
    CHECK_VALUE(dst_ptr->data, FALSE);
    const u8* src_data = src_ptr->data;
    const u8* dst_data = dst_ptr->data;
    if (dst_data != src_data) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_left(const_vm_ptr cvm, u64 src, u64 shift) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(shift, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    CHECK_CONDITION(offset < shift, NULL_ADDRESS);
    offset -= shift;
    u64 address = create_string_reference_internal(cvm, const_ptr, offset);
    return address;
}

static u64 string_right(const_vm_ptr cvm, u64 src, u64 nbytes) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(nbytes, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size - 1;
    CHECK_CONDITION(offset + nbytes >= size, NULL_ADDRESS);
    offset += nbytes;
    u64 address = create_string_reference_internal(cvm, const_ptr, offset);
    return address;
}

static u64 string_left_copy(const_vm_ptr cvm, u64 src, u64 nbytes) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(nbytes, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    CHECK_CONDITION(offset < nbytes, NULL_ADDRESS);
    const u8* ch = const_ptr->data;
    ch += offset - nbytes;
    u64 address = CALL(pointer)->copy(cvm, ch, nbytes + 1, 0, string_type_definitions.type_id);
    return address;
}

static u64 string_right_copy(const_vm_ptr cvm, u64 src, u64 nbytes) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(src, NULL_ADDRESS);
    CHECK_ARG(nbytes, NULL_ADDRESS);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, src, &const_offset);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    CHECK_VALUE(const_ptr->data, NULL_ADDRESS);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size - 1;
    CHECK_CONDITION(offset + nbytes >= size, NULL_ADDRESS);
    const u8* ch = const_ptr->data;
    ch += offset;
    u64 address = CALL(pointer)->copy(cvm, ch, nbytes + 1, 0, string_type_definitions.type_id);
    return address;
}

static u64 string_move_left(const_vm_ptr cvm, u64 address, u64 nbytes) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    CHECK_ARG(nbytes, FALSE);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, address, &const_offset);
    CHECK_POINTER(const_ptr, FALSE);
    CHECK_VALUE(const_ptr->data, FALSE);
    u64 offset = const_offset;
    CHECK_CONDITION(offset < nbytes, FALSE);
    safe_pointer_ptr src_ptr;
    src_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = src_ptr.ptr;
    ptr->offset -= nbytes;
    return TRUE;
}

static u64 string_move_right(const_vm_ptr cvm, u64 address, u64 nbytes) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    CHECK_ARG(nbytes, FALSE);
    u64 const_offset = 0;
    const_pointer_ptr const_ptr = read_offset_internal(cvm, address, &const_offset);
    CHECK_POINTER(const_ptr, FALSE);
    CHECK_VALUE(const_ptr->data, FALSE);
    u64 offset = const_offset;
    u64 size = const_ptr->public.size - 1;
    CHECK_CONDITION(offset + nbytes >= size, FALSE);
    safe_pointer_ptr src_ptr;
    src_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = src_ptr.ptr;
    ptr->offset += nbytes;
    return TRUE;
}

static u64 string_strcmp(const_vm_ptr cvm, u64 src, u64 dst) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    CHECK_ARG(dst, FALSE);
    CHECK_CONDITION(src == dst, FALSE);
    const_pointer_ptr src_ptr = read_internal(cvm, src);
    CHECK_POINTER(src_ptr, FALSE);
    CHECK_VALUE(src_ptr->data, FALSE);
    const_pointer_ptr dst_ptr = read_internal(cvm, dst);
    CHECK_POINTER(dst_ptr, FALSE);
    CHECK_VALUE(dst_ptr->data, FALSE);
    const u8* src_data = src_ptr->data;
    const u8* dst_data = dst_ptr->data;
    if (CALL(os)->strcmp((const char*)src_data, (const char*)dst_data) != 0) {
        return FALSE;
    }
    return TRUE;
}

static u64 string_ref(const_vm_ptr cvm, u64 src, u64 depth) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(src, FALSE);
    u64 search_depth = (depth == 0) ? MAX_DEPTH : depth + 1;
    const_pointer_ptr src_ptr = CALL(pointer)->read(cvm, src, string_type_definitions.type_id);
    CHECK_POINTER(src_ptr, FALSE);
    while (IS_FLAG_MEMORY_ADDRESS(src_ptr->flags) && search_depth-- > 0) {
        src_ptr = CALL(pointer)->read(cvm, (u64)src_ptr->data, string_type_definitions.type_id);
        CHECK_POINTER(src_ptr, FALSE);
        CHECK_VALUE(src_ptr->data, FALSE);
    }
    u64 address = src_ptr->public.address;
    return address;
}

/* public */
CVM_EXPORT void string_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &string_type_definitions;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_string_methods PRIVATE_API(virtual_string_methods_definitions) = {
    .free = string_free,
    .split = string_split,
    .copy = string_copy,
    .strncpy = string_strncpy,
    .strcat = string_strcat,
    .strrchr = string_strrchr,
    .strchr = string_strchr,
    .match = string_match,
    .match_offset = string_match_offset,
    .load = string_load,
    .put_char = string_put_char,
    .unsafe = string_unsafe,
    .size = string_size,
    .lessthan = string_lessthan,
    .greaterthan = string_greaterthan,
    .equals = string_equals,
    .compare = string_compare,
    .left = string_left,
    .right = string_right,
    .left_copy = string_left_copy,
    .right_copy = string_right_copy,
    .move_left = string_move_left,
    .move_right = string_move_right,
    .strcmp = string_strcmp,
    .ref = string_ref
};

const virtual_string_methods* PRIVATE_API(string) = &PRIVATE_API(virtual_string_methods_definitions);
const virtual_string_methods* CALL(string) {
    return PRIVATE_API(string);
}
