/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 7, 2025 at 3:01:17 PM GMT+3
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

#include "std/api.h"

#include "std/data.h"

#include "virtual/api/api_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100
#define STRING_REFERENCE_SIZE sizeof(string_reference_type)

static const enum type id = TYPE_STRING;

/* internal */
static char* string_pointer_internal(const_pointer_ptr data_ptr, u64* data_size, u64* data_offset);
static char* string_strrchr_internal(char* ch, const char* str2, u64 size, u64 offset);
static char* string_strchr_internal(char* ch, const char* str2, u64 size, u64 offset);

/* definition */
static void string_free(u64 ptr);
static u64 string_copy(u64 ptr);
static void string_strcpy(u64 dest_ptr, u64 src_ptr);
static void string_strcat(u64 dest_ptr, u64 src_ptr);
static u64 string_strrchr(u64 src_ptr, u64 match_ptr);
static u64 string_strchr(u64 src_ptr, u64 match_ptr);
static u64 string_match(u64 src_ptr, u64 match_ptr);
static u64 string_offset(u64 src_ptr, u64 match_ptr);
static u64 string_load(const char* ch);
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
static const struct type_methods_definitions string_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    CALL(pointer)->register_known_type(id, &string_type);
}

static void type_desctructor(const_pointer_ptr const_ptr) {
    CALL(pointer)->release(const_ptr);
}

/* implementation */
static char* string_pointer_internal(const_pointer_ptr data_ptr, u64* data_size, u64* data_offset) {
    u64 offset = 0;
    const_pointer_ptr const_ptr;
    u64 str_ptr1 = CALL(pointer)->read_type(data_ptr, TYPE_STRING);
    if (str_ptr1) {
        const_ptr = data_ptr;
        *data_size = CALL(pointer)->size(const_ptr);
        *data_offset = offset;
        char* ch = CALL(pointer)->read(const_ptr);
        return ch;
    }
    u64 str_ptr2 = CALL(pointer)->read_type(data_ptr, TYPE_STRING_POINTER);
    if (str_ptr2) {
        const_string_reference_ptr ref = CALL(pointer)->read(data_ptr);
        while ((const_ptr = CALL(virtual)->read(ref->address)) != 0) {
            if (!CALL(pointer)->read_type(const_ptr, TYPE_STRING_POINTER)) {
                break;
            }
            offset += ref->offset;
            ref = CALL(pointer)->read(const_ptr);
        }
        const_ptr = CALL(virtual)->read_type(ref->address, TYPE_STRING);
        if (const_ptr == 0) {
            return 0;
        }
        offset += ref->offset;
        *data_size = CALL(pointer)->size(const_ptr);
        *data_offset = offset;
        char* ch = CALL(pointer)->read(const_ptr);
        return ch;
    }
    return 0;
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
        return 0;
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
        return 0;
    }
    str1 = last;
    return str1;
}

/* api */
static void string_free(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read(ptr);
    if (data_ptr == 0) {
        return;
    }
    u64 str_ptr1 = CALL(pointer)->read_type(data_ptr, TYPE_STRING);
    if (str_ptr1) {
        type_desctructor(data_ptr);
        return;
    }
    u64 str_ptr2 = CALL(pointer)->read_type(data_ptr, TYPE_STRING_POINTER);
    if (str_ptr2) {
        const_string_reference_ptr ref = CALL(pointer)->read(data_ptr);
        const_pointer_ptr p_ptr = CALL(virtual)->read_type(ref->address, TYPE_STRING_POINTER);
        if (p_ptr == 0) {
            return;
        }
        type_desctructor(p_ptr);
        type_desctructor(data_ptr);
        return;
    }
}

static u64 string_copy(u64 src) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    ch += offset;
    u64 result = CALL(virtual)->memcpy(size - offset, ch, id);
    return result;
}

static void string_strcpy(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    pointer_ptr dest_ptr = CALL(virtual)->read_type(dest, id);
    if (dest_ptr == 0) {
        return;
    }
    const_pointer_ptr src_ptr = CALL(virtual)->read(src);
    if (src_ptr == 0) {
        return;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const char* ch = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (ch == 0) {
        return;
    }
    ch += src_offset;
    u64 dest_size = CALL(pointer)->size(dest_ptr);
    if (dest_size < src_size) {
        CALL(pointer)->realloc(dest_ptr, src_size);
    }
    u64 size = src_size - src_offset;
    char* data = CALL(pointer)->read_guard(dest_ptr, size - 1);
    virtual_api->strncpy(data, ch, size); /* NOLINT: strncpy is safe here because the destination buffer is guaranteed to be large enough */
}

static void string_strcat(u64 dest, u64 src) {
    if (src == dest) {
        return;
    }
    pointer_ptr dest_ptr = CALL(virtual)->read_type(dest, id);
    if (dest_ptr == 0) {
        return;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read(src);
    if (data_ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(data_ptr, &size, &offset);
    if (ch == 0) {
        return;
    }
    const char* data_src = ch + offset; /* NOLINT: memory bounds check */
    u64 dest_size = CALL(pointer)->size(dest_ptr);
    size += dest_size - 1;
    if (dest_size < size) {
        CALL(pointer)->realloc(dest_ptr, size);
    }
    char* data = CALL(pointer)->read_guard(dest_ptr, size - 1);
    virtual_api->strncat(data, data_src, size - dest_size); /* NOLINT: strncat is safe here because the destination buffer is guaranteed to be large enough */
}

static u64 string_strrchr(u64 src, u64 match) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    char* text = ch + offset;
    const char* str2 = CALL(pointer)->read(match_ptr);
    const char* str1 = string_strrchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        return 0;
    }
    ch += offset;
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_strchr(u64 src, u64 match) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    char* text = ch + offset;
    const char* str2 = CALL(pointer)->read(match_ptr);
    const char* str1 = string_strchr_internal(text, str2, size, offset);
    if (str1 == 0) {
        return 0;
    }
    ch += offset;
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_match(u64 src, u64 match) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    ch += offset;
    u64 match_size = CALL(pointer)->size(match_ptr);
    char* str2 = CALL(pointer)->read(match_ptr);
    char* str1 = ch;
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
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static char* get_last_match(char* str1, const char* str2, u64* size, u64 offset) {
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

static u64 string_offset(u64 src, u64 match) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    const_pointer_ptr match_ptr = CALL(virtual)->read_type(match, id);
    if (match_ptr == 0) {
        return 0;
    }
    ch += offset;
    u64 match_size = CALL(pointer)->size(match_ptr);
    char* str2 = CALL(pointer)->read(match_ptr);
    char* str1 = get_last_match(ch, str2, &size, offset);
    if (str1 == 0) {
        return 0;
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
    offset = (u64)(str1 - ch);
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->read(data_ptr);
    ref->address = src;
    ref->offset = offset;
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_load(const char* ch) {
    if (ch == 0) {
        return 0;
    }
    if (*ch == 0) {
        return 0;
    }
    u64 size = virtual_api->strlen(ch) + 1;
    const_pointer_ptr data_ptr = CALL(pointer)->copy_guard(ch, size, size - 1, id);
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static void string_put_char(u64 src, char value) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return;
    }
    u64 size = 0;
    u64 offset = 0;
    char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return;
    }
    ch += offset;
    *ch = value;
}

static char* string_unsafe(u64 src) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    ch += offset;
    return ch;
}

static u64 string_size(u64 src) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    return size - 1;
}

static u64 string_lessthan(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    const_pointer_ptr src_ptr = CALL(virtual)->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    const_pointer_ptr dst_ptr = CALL(virtual)->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    if (src_offset >= dst_offset) {
        return 0;
    }
    return dst_offset - src_offset;
}

static u64 string_greaterthan(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    const_pointer_ptr src_ptr = CALL(virtual)->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    const_pointer_ptr dst_ptr = CALL(virtual)->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    if (src_offset <= dst_offset) {
        return 0;
    }
    return src_offset - dst_offset;
}

static u64 string_equals(u64 src, u64 dst) {
    if (src == dst) {
        return 0;
    }
    const_pointer_ptr src_ptr = CALL(virtual)->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    const_pointer_ptr dst_ptr = CALL(virtual)->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
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
    const_pointer_ptr src_ptr = CALL(virtual)->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    const_pointer_ptr dst_ptr = CALL(virtual)->read(dst);
    if (dst_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    u64 dst_size = 0;
    u64 dst_offset = 0;
    const char* dst_data = string_pointer_internal(dst_ptr, &dst_size, &dst_offset);
    if (dst_data == 0) {
        return 0;
    }
    if (dst_data != src_data) {
        return 0;
    }
    return (u64)(0 - 1);
}

static u64 string_left(u64 src, u64 shift) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    if (offset < shift) {
        return 0;
    }
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->read(data_ptr);
    ref->address = src;
    ref->offset = 0 - shift;
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_strncpy(u64 src, u64 nbytes) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    if (offset + nbytes >= size) {
        return 0;
    }
    ch += offset;
    size = nbytes + 1;
    const_pointer_ptr data_ptr = CALL(pointer)->copy_guard(ch, size, size - 1, id);
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_left_strncpy(u64 src, u64 nbytes) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    if (offset < nbytes) {
        return 0;
    }
    ch += (offset - nbytes);
    size = nbytes + 1;
    const_pointer_ptr data_ptr = CALL(pointer)->copy_guard(ch, size, size - 1, id);
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_right(u64 src, u64 nbytes) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    if (offset + nbytes >= size) {
        return 0;
    }
    const_pointer_ptr data_ptr = CALL(pointer)->alloc(STRING_REFERENCE_SIZE, TYPE_STRING_POINTER);
    string_reference_ptr ref = CALL(pointer)->read(data_ptr);
    ref->address = src;
    ref->offset = nbytes;
    u64 result = CALL(virtual)->alloc(data_ptr);
    return result;
}

static u64 string_move_left(u64 src, u64 nbytes) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    if (!CALL(pointer)->read_type(const_ptr, TYPE_STRING_POINTER)) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    if (offset < nbytes) {
        return 0;
    }
    string_reference_ptr ref = CALL(pointer)->read(const_ptr);
    ref->offset -= nbytes;
    return (u64)(0 - 1);
}

static u64 string_move_right(u64 src, u64 nbytes) {
    const_pointer_ptr const_ptr = CALL(virtual)->read(src);
    if (const_ptr == 0) {
        return 0;
    }
    u64 size = 0;
    u64 offset = 0;
    if (!CALL(pointer)->read_type(const_ptr, TYPE_STRING_POINTER)) {
        return 0;
    }
    const char* ch = string_pointer_internal(const_ptr, &size, &offset);
    if (ch == 0) {
        return 0;
    }
    if (offset + nbytes >= size) {
        return 0;
    }
    string_reference_ptr ref = CALL(pointer)->read(const_ptr);
    ref->offset += nbytes;
    return (u64)(0 - 1);
}

static u64 string_strcmp(u64 src, u64 dest) {
    if (src == dest) {
        return 0;
    }
    const_pointer_ptr src_ptr = CALL(virtual)->read(src);
    if (src_ptr == 0) {
        return 0;
    }
    u64 src_size = 0;
    u64 src_offset = 0;
    const char* src_data = string_pointer_internal(src_ptr, &src_size, &src_offset);
    if (src_data == 0) {
        return 0;
    }
    const_pointer_ptr dest_ptr = CALL(virtual)->read(dest);
    if (dest_ptr == 0) {
        return 0;
    }
    u64 dest_size = 0;
    u64 dest_offset = 0;
    const char* dest_data = string_pointer_internal(dest_ptr, &dest_size, &dest_offset);
    if (dest_data == 0) {
        return 0;
    }
    if (virtual_api->strcmp(src_data, dest_data) != 0) {
        return 0;
    }
    return (u64)(0 - 1);
}

#ifndef ATTRIBUTE
void string_init(void) {
    init();
}
#endif

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
