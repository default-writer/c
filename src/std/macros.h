/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 11:18:55 AM GMT+3
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

#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define USING_MACROS

#define TRUE 1
#define FALSE 0
#define NULL_PTR (void_ptr)0

#define VM_EXPAND(X) X
#define VM_CONCAT(A, B) VM_CONCAT_(A, B)
#define VM_CONCAT_(A, B) A##B
#define VM_STRINGIFY_(X) #X
#define VM_STRINGIFY(X) VM_STRINGIFY_(X)

#define API_VERSION VM_EXPAND(VM_CONCAT(, VM_STRINGIFY(VM_CONCAT(MAJOR_VERSION, VM_CONCAT(., VM_CONCAT(MINOR_VERSION, VM_CONCAT(., PATCH_VERSION)))))))
#define API(x) VM_CONCAT(x, VM_CONCAT(_, VM_CONCAT(v, VM_CONCAT(MAJOR_VERSION, VM_CONCAT(_, VM_CONCAT(MINOR_VERSION, VM_CONCAT(_, PATCH_VERSION)))))))
#define CURRENT_API(x) VM_CONCAT(API(x), VM_CONCAT(_, GIT_COMMIT_HASH_VALUE))
#define CALL(x) CURRENT_API(x)()

#define PRIVATE_API(x) _##x
#define PUBLIC_API(x) x()

#define PTR_SIZE sizeof(void_ptr) /* size of a pointer */
#define PTR_ARRAY_SIZE(size) (size * PTR_SIZE)

#ifdef USE_MEMORY_DEBUG_INFO
#define ERROR(message_id, format, ...)                                                                                                                                                  \
    {                                                                                                                                                                                   \
        int _##message_id##_snp_format_size = snprintf(NULL, 0, format ": %s (%s:%d)", __VA_ARGS__, __func__, __FILE__, __LINE__); /* NOLINT: snprintf(NULL) */                         \
        char _##message_id##_snp_format_buffer[_##message_id##_snp_format_size + 1];                                                                                                    \
        snprintf(_##message_id##_snp_format_buffer, sizeof _##message_id##_snp_format_buffer, format ": %s (%s:%d)", __VA_ARGS__, __func__, __FILE__, __LINE__); /* NOLINT: snprintf */ \
        CALL(error)->output(stderr, ID_##message_id, _##message_id##_snp_format_buffer, (u64)_##message_id##_snp_format_size);                                                          \
        CALL(error)->throw(ID_##message_id, _##message_id##_snp_format_buffer, (u64)_##message_id##_snp_format_size);                                                                   \
    }
#define ERROR_NO_ERROR(format, ...) ERROR(ERROR_NO_ERROR, format, __VA_ARGS__)
#define ERROR_VM_NOT_INITIALIZED(format, ...) ERROR(ERROR_VM_NOT_INITIALIZED, format, ##__VA_ARGS__)
#define ERROR_INVALID_POINTER(format, ...) ERROR(ERROR_INVALID_POINTER, format, __VA_ARGS__)
#define ERROR_INVALID_ARGUMENT(format, ...) ERROR(ERROR_INVALID_ARGUMENT, format, __VA_ARGS__)
#define ERROR_INVALID_TYPE_ID(format, ...) ERROR(ERROR_INVALID_TYPE_ID, format, __VA_ARGS__)
#define ERROR_INVALID_VALUE(format, ...) ERROR(ERROR_INVALID_VALUE, format, __VA_ARGS__)
#else
#define ERROR(message_id, format, ...)                                                                                                     \
    {                                                                                                                                      \
        int _##message_id##_snp_format_size = snprintf(NULL, 0, format, __VA_ARGS__); /* NOLINT: snprintf(NULL) */                         \
        char _##message_id##_snp_format_buffer[_##message_id##_snp_format_size + 1];                                                       \
        snprintf(_##message_id##_snp_format_buffer, sizeof _##message_id##_snp_format_buffer, format, __VA_ARGS__); /* NOLINT: snprintf */ \
        CALL(error)->output(stderr, ID_##message_id, _##message_id##_snp_format_buffer, (u64)_##message_id##_snp_format_size);             \
        CALL(error)->throw(ID_##message_id, _##message_id##_snp_format_buffer, (u64)_##message_id##_snp_format_size);                      \
    }
#define ERROR_NO_ERROR(format, ...) ERROR(ERROR_NO_ERROR, format, __VA_ARGS__)
#define ERROR_VM_NOT_INITIALIZED(format, ...) ERROR(ERROR_VM_NOT_INITIALIZED, format, __VA_ARGS__)
#define ERROR_INVALID_POINTER(format, ...) ERROR(ERROR_INVALID_POINTER, format, __VA_ARGS__)
#define ERROR_INVALID_ARGUMENT(format, ...) ERROR(ERROR_INVALID_ARGUMENT, format, __VA_ARGS__)
#define ERROR_INVALID_TYPE_ID(format, ...) ERROR(ERROR_INVALID_TYPE_ID, format, __VA_ARGS__)
#define ERROR_INVALID_VALUE(format, ...) ERROR(ERROR_INVALID_VALUE, format, __VA_ARGS__)
#endif

#ifdef __GNUC__
#define FALL_THROUGH __attribute__((fallthrough))
#define ATTRIBUTE
// #define INIT __attribute__((constructor))
// #define DESTROY __attribute__((destructor))
#define PUBLIC __attribute__((visibility("default")))
#else
#define FALL_THROUGH ((void)0)
#define ATTRIBUTE
// #define INIT
// #define DESTROY
#define PUBLIC
#endif /* __GNUC__ >= 7 */

#endif /* _STD_MACROS_H_ */
