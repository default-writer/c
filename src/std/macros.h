/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 1, 2025 at 9:22:21 AM GMT+3
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

#define USING_MACROS

#ifndef STD_MACROS_H
#define STD_MACROS_H

#define TRUE 1
#define FALSE 0
#define NULL_PTR (void_ptr)0
#define NULL_ADDRESS (u64)0
#define NULL_VALUE (u64)0

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
#define PTR_ARRAY_SIZE(size) ((size) * PTR_SIZE)

#ifdef USE_MEMORY_DEBUG_INFO
#define STD_VM_ERROR(message_id, format, ...)                                                                                                           \
    do {                                                                                                                                               \
        int snp_format_size = snprintf(NULL, 0, format ": %s (%s:%d)", __VA_ARGS__, __func__, __FILE__, __LINE__); /* NOLINT */      \
        char* snp_format_buffer = (char*)malloc(snp_format_size + 1);                                                   \
        if (snp_format_buffer) {                                                                                                       \
            snprintf(snp_format_buffer, snp_format_size + 1, format ": %s (%s:%d)", __VA_ARGS__, __func__, __FILE__, __LINE__); \
            CALL(error)->output(stderr, ID_##message_id, snp_format_buffer, (u64)snp_format_size);                      \
            CALL(error)->throw(ID_##message_id, snp_format_buffer, (u64)snp_format_size);                               \
            free(snp_format_buffer);                                                                                                   \
        }                                                                                                                                              \
    } while (0)
#else
#define STD_VM_ERROR(message_id, format, ...)                                                                                                           \
    do {                                                                                                                                               \
        int snp_format_size = snprintf(NULL, 0, format, __VA_ARGS__); /* NOLINT */                                                  \
        char* snp_format_buffer = (char*)malloc(snp_format_size + 1);                                                   \
        if (snp_format_buffer) {                                                                                                       \
            snprintf(snp_format_buffer, snp_format_size + 1, format, __VA_ARGS__);                                  \
            CALL(error)->output(stderr, ID_##message_id, snp_format_buffer, (u64)snp_format_size);                      \
            CALL(error)->throw(ID_##message_id, snp_format_buffer, (u64)snp_format_size);                               \
            free(snp_format_buffer);                                                                                                   \
        }                                                                                                                                              \
    } while (0)
#endif

#define ERROR_NO_STD_VM_ERROR(format, ...) STD_VM_ERROR(ERROR_NO_ERROR, format, __VA_ARGS__)
#define ERROR_VM_NOT_INITIALIZED(format, ...) STD_VM_ERROR(ERROR_VM_NOT_INITIALIZED, format, ##__VA_ARGS__)
#define ERROR_INVALID_POINTER(format, ...) STD_VM_ERROR(ERROR_INVALID_POINTER, format, __VA_ARGS__)
#define ERROR_INVALID_ARGUMENT(format, ...) STD_VM_ERROR(ERROR_INVALID_ARGUMENT, format, __VA_ARGS__)
#define ERROR_INVALID_CONDITION(format, ...) STD_VM_ERROR(ERROR_INVALID_CONDITION, format, __VA_ARGS__)
#define ERROR_INVALID_TYPE_ID(format, ...) STD_VM_ERROR(ERROR_INVALID_TYPE_ID, format, __VA_ARGS__)
#define ERROR_INVALID_VALUE(format, ...) STD_VM_ERROR(ERROR_INVALID_VALUE, format, __VA_ARGS__)

#ifdef USE_MEMORY_DEBUG_INFO
#define CHECK_VM_CONDITION(condition, null)             \
    do {                                                \
        if (condition) {                                \
            ERROR_VM_NOT_INITIALIZED("%s", #condition); \
            return null;                                \
        }                                               \
    } while (0)
#define CHECK_VM(cvm, null)                       \
    do {                                          \
        if (cvm == 0 || *cvm == 0) {              \
            ERROR_VM_NOT_INITIALIZED("%s", #cvm); \
            return null;                          \
        }                                         \
    } while (0)
#define CHECK_VM_NO_RETURN(cvm)                   \
    do {                                          \
        if (cvm == 0 || *cvm == 0) {              \
            ERROR_VM_NOT_INITIALIZED("%s", #cvm); \
            return;                               \
        }                                         \
    } while (0)
#define CHECK_ARG(arg, null)                    \
    do {                                        \
        if (arg == 0) {                         \
            ERROR_INVALID_ARGUMENT("%s", #arg); \
            return null;                        \
        }                                       \
    } while (0)
#define CHECK_ARG_NO_RETURN(arg)                \
    do {                                        \
        if (arg == 0) {                         \
            ERROR_INVALID_ARGUMENT("%s", #arg); \
            return;                             \
        }                                       \
    } while (0)
#define CHECK_POINTER(arg, null)               \
    do {                                       \
        if (arg == 0) {                        \
            ERROR_INVALID_POINTER("%s", #arg); \
            return null;                       \
        }                                      \
    } while (0)
#define CHECK_POINTER_NO_RETURN(arg)           \
    do {                                       \
        if (arg == 0) {                        \
            ERROR_INVALID_POINTER("%s", #arg); \
            return;                            \
        }                                      \
    } while (0)
#define CHECK_CONDITION(condition, null)               \
    do {                                               \
        if (condition) {                               \
            ERROR_INVALID_CONDITION("%s", #condition); \
            return null;                               \
        }                                              \
    } while (0)
#define CHECK_CONDITION_NO_RETURN(condition)           \
    do {                                               \
        if (condition) {                               \
            ERROR_INVALID_CONDITION("%s", #condition); \
            return;                                    \
        }                                              \
    } while (0)
#define CHECK_VALUE(value, null)               \
    do {                                       \
        if (value == 0) {                      \
            ERROR_INVALID_VALUE("%s", #value); \
            return null;                       \
        }                                      \
    } while (0)
#define CHECK_VALUE_NO_RETURN(value)           \
    do {                                       \
        if (value == 0) {                      \
            ERROR_INVALID_VALUE("%s", #value); \
            return;                            \
        }                                      \
    } while (0)
#define CHECK_TYPE(condition, null)                  \
    do {                                             \
        if (condition) {                             \
            ERROR_INVALID_TYPE_ID("%s", #condition); \
            return null;                             \
        }                                            \
    } while (0)
#define CHECK_TYPE_NO_RETURN(condition)              \
    do {                                             \
        if (condition) {                             \
            ERROR_INVALID_TYPE_ID("%s", #condition); \
            return;                                  \
        }                                            \
    } while (0)
#else
#define CHECK_VM_CONDITION(condition, null) \
    do {                                    \
        if (condition) {                    \
            return null;                    \
        }                                   \
    } while (0)
#define CHECK_VM(cvm, null)          \
    do {                             \
        if (cvm == 0 || *cvm == 0) { \
            return null;             \
        }                            \
    } while (0)
#define CHECK_VM_NO_RETURN(cvm)      \
    do {                             \
        if (cvm == 0 || *cvm == 0) { \
            return;                  \
        }                            \
    } while (0)
#define CHECK_ARG(arg, null) \
    do {                     \
        if (arg == 0) {      \
            return null;     \
        }                    \
    } while (0)
#define CHECK_ARG_NO_RETURN(arg) \
    do {                         \
        if (arg == 0) {          \
            return;              \
        }                        \
    } while (0)
#define CHECK_POINTER(arg, null) \
    do {                         \
        if (arg == 0) {          \
            return null;         \
        }                        \
    } while (0)
#define CHECK_POINTER_NO_RETURN(arg) \
    do {                             \
        if (arg == 0) {              \
            return;                  \
        }                            \
    } while (0)
#define CHECK_CONDITION(condition, null) \
    do {                                 \
        if (condition) {                 \
            return null;                 \
        }                                \
    } while (0)
#define CHECK_CONDITION_NO_RETURN(condition) \
    do {                                     \
        if (condition) {                     \
            return;                          \
        }                                    \
    } while (0)
#define CHECK_VALUE(value, null) \
    do {                         \
        if (value == 0) {        \
            return null;         \
        }                        \
    } while (0)
#define CHECK_VALUE_NO_RETURN(value) \
    do {                             \
        if (value == 0) {            \
            return;                  \
        }                            \
    } while (0)
#define CHECK_TYPE(condition, null) \
    do {                            \
        if (condition) {            \
            return null;            \
        }                           \
    } while (0)
#define CHECK_TYPE_NO_RETURN(condition) \
    do {                                \
        if (condition) {                \
            return;                     \
        }                               \
    } while (0)
#endif

#define FLAG_NONE (u64)0x0000000000000000
#define FLAG_MEMORY_PTR (u64)0x0000000000000001
#define FLAG_MEMORY_ADDRESS (u64)0x0000000000000002
#define IS_FLAG_MEMORY_PTR(flags) (((flags) & FLAG_MEMORY_PTR) == FLAG_MEMORY_PTR)
#define IS_FLAG_MEMORY_ADDRESS(flags) (((flags) & FLAG_MEMORY_ADDRESS) == FLAG_MEMORY_ADDRESS)
#define MAX_DEPTH (u64)0xffffffffffffffff

#ifdef __GNUC__
#define FALL_THROUGH __attribute__((fallthrough))
#define ATTRIBUTE
// #define INIT __attribute__((constructor))
// #define DESTROY __attribute__((destructor))
#define PUBLIC inline __attribute__((always_inline, visibility("default")))
#define INLINE inline __attribute__((always_inline))
#else
#define FALL_THROUGH ((void)0)
#define ATTRIBUTE
// #define INIT
// #define DESTROY
#define PUBLIC
#define INLINE inline
#endif /* __GNUC__ >= 7 */

#endif /* STD_MACROS_H */
