/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 27, 2025 at 9:09:07 PM GMT+3
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

#include "file_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"

#include "internal/pointer_type_v1.h"

#define FILE_HANDLER_TYPE_SIZE sizeof(file_handler_type)

/* definition */
typedef struct file_handler* file_handler_ptr;
typedef struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* path;
#endif
} file_handler_type;

/* public */
static u64 file_alloc(const_vm_ptr cvm, u64 file_path_ptr, u64 mode_ptr);
static u64 file_free(const_vm_ptr cvm, u64 address);
static u64 file_data(const_vm_ptr cvm, u64 address);

/* type */
static void file_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions file_type_definitions = {
    .type_id = TYPE_FILE,
    .destructor = file_type_destructor
};

static void file_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, file_type_definitions.type_id);
    CHECK_POINTER_NO_RETURN(const_ptr);
    CALL(pointer)->free(cvm, address);
}

static int is_valid_file_path(const char* file_path) {
    size_t size = CALL(os)->strlen(file_path);
    if (size > 0 && size <= PATH_MAX - 1) {
        return TRUE;
    }
    return FALSE;
}

static u64 is_valid_mode_internal(const char* mode) {
    const char* valid_modes[] = { "r", "w", "a", "r+", "w+", "a+", "rb", "wb", "ab", "r+b", "w+b", "a+b" };
    size_t num_modes = sizeof(valid_modes) / sizeof(valid_modes[0]);
    u64 result = FALSE;
    for (size_t i = 0; i < num_modes; ++i) {
        if (CALL(os)->strcmp(mode, valid_modes[i]) == 0) {
            result = TRUE;
            break;
        }
    }
    return result;
}

static u64 file_alloc(const_vm_ptr cvm, u64 path, u64 mode) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(path, NULL_ADDRESS);
    CHECK_ARG(mode, NULL_ADDRESS);
    const_pointer_ptr path_const_ptr = CALL(pointer)->read(cvm, path, TYPE_STRING);
    CHECK_POINTER(path_const_ptr, NULL_ADDRESS);
    const char* file_path_data = path_const_ptr->data;
    CHECK_VALUE(is_valid_file_path(file_path_data), NULL_ADDRESS);
    const_pointer_ptr mode_pointer_ptr = CALL(pointer)->read(cvm, mode, TYPE_STRING);
    CHECK_POINTER(mode_pointer_ptr, NULL_ADDRESS);
    const char* mode_data = mode_pointer_ptr->data;
    CHECK_VALUE(is_valid_mode_internal(mode_data), NULL_ADDRESS);
    FILE* f = CALL(os)->fopen(file_path_data, mode_data); /* NOLINT */
    CHECK_VALUE(f, NULL_ADDRESS);
    void_ptr data = CALL(memory)->alloc(FILE_HANDLER_TYPE_SIZE);
    u64 address = CALL(pointer)->alloc(cvm, data, FILE_HANDLER_TYPE_SIZE, 0, FLAG_MEMORY_PTR, file_type_definitions.type_id);
    file_handler_ptr handler = data;
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = file_path_data;
#endif
    return address;
}

static u64 file_data(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, file_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    file_handler_ptr handler = data_ptr;
    FILE* f = handler->file;
    if (CALL(os)->fseek(f, 0, SEEK_END) == 0) { /* NOLINT */
        long size = CALL(os)->ftell(f);
        if (size >= 0) {
            u64 data_size = (u64)size;
            u64 data_handle = CALL(data)->alloc(cvm, data_size + 1);
            void_ptr file_data = CALL(data)->unsafe(cvm, data_handle);
            if (CALL(os)->fseek(f, 0, SEEK_SET) == 0) { /* NOLINT */
                u64 read = CALL(os)->fread(file_data, 1, data_size, handler->file);
                if (handler->file != 0) {
                    CALL(os)->fclose(handler->file);
                    handler->file = 0;
                }
                if (read == data_size) {
                    return data_handle;
                }
            }
        }
    }
    return NULL_ADDRESS;
}

static u64 file_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    file_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void file_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &file_type_definitions;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

/*! definition (initialization) of file_methods structure */
const virtual_file_methods PRIVATE_API(virtual_file_methods_definitions) = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

const virtual_file_methods* PRIVATE_API(file) = &PRIVATE_API(virtual_file_methods_definitions);
const virtual_file_methods* CALL(file) {
    return PRIVATE_API(file);
}
