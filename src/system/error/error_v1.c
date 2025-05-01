/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 1, 2025 at 3:54:06 PM GMT+3
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

#include "error_v1.h"

#include "system/os/os_v1.h"

/* private */
static const char* error_messages[] = {
    [ID_ERROR_NO_ERROR] = "no error",
    [ID_ERROR_VM_NOT_INITIALIZED] = "vm not initialized",
    [ID_ERROR_INVALID_POINTER] = "invalid pointer",
    [ID_ERROR_INVALID_ARGUMENT] = "invalid argument",
    [ID_ERROR_INVALID_CONDITION] = "invalid condition",
    [ID_ERROR_INVALID_TYPE_ID] = "invalid type id",
    [ID_ERROR_INVALID_VALUE] = "invalid value",
};

static exception_type exception;
static exception_ptr exception_errors = &exception;

#ifndef USE_MEMORY_DEBUG_INFO
static void error_output(FILE* output, u64 error_type, const char* message, u64 size);
static void error_throw(u64 error_type, const char* message, u64 size);
#endif
static void error_clear(void);
static u64 error_type(void);
static FILE* error_std_vm_out(void);
static FILE* error_std_vm_err(void);
static const char* error_get(void);
static u64 error_next(void);
static u64 error_count(void);

/* implementation */
static void error_output(FILE* output, u64 error_type, const char* message, u64 size) {
#ifdef USE_TTY
    if (isatty(STDERR_FILENO)) {
        const char* start = "\x1b[31m";
        const char* end = "\x1b[0m";
        fprintf(output, "%s[debug]%s: %s: %s\n", start, end, error_messages[error_type], message); /* NOLINT: fprintf(output) */
    }
#else
    fprintf(output, "[debug]: %s: %s\n", error_messages[error_type], message); /* NOLINT: fprintf(output) */
#endif
}

static void error_throw(u64 error_type, const char* message, u64 size) {
    if (exception_errors->message_count < ERROR_MESSAGE_COUNT) {
        exception_errors->message_count++;
    } else {
        CALL(os)->memmove(&exception_errors->type[0], &exception_errors->type[1], (exception_errors->message_count - 1) * sizeof(u64));
        CALL(os)->memmove(&exception_errors->message[0], &exception_errors->message[ERROR_MESSAGE_SIZE], (exception_errors->message_count - 1) * ERROR_MESSAGE_SIZE);
    }
    exception_errors->type[exception_errors->message_count - 1] = error_type;
    if (size < ERROR_MESSAGE_SIZE) {
        CALL(os)->memcpy(&exception_errors->message[(exception_errors->message_count - 1) * ERROR_MESSAGE_SIZE], message, size); /* NOLINT: memcpy(ex->message */
        CALL(os)->memset(&exception_errors->message[(exception_errors->message_count - 1) * ERROR_MESSAGE_SIZE + size], 0x00, ERROR_MESSAGE_SIZE - size);
    } else {
        CALL(os)->memcpy(&exception_errors->message[(exception_errors->message_count - 1) * ERROR_MESSAGE_SIZE], message, ERROR_MESSAGE_SIZE - 1); /* NOLINT: memcpy(ex->message */
        exception_errors->message[(exception_errors->message_count - 1) * ERROR_MESSAGE_SIZE + ERROR_MESSAGE_SIZE - 1] = 0;
    }
}

static void error_clear(void) {
    if (exception_errors->message_count != 0) {
        CALL(os)->memset(&exception_errors->type[0], 0x00, ERROR_MESSAGE_COUNT);
        CALL(os)->memset(&exception_errors->message[0], 0x00, ERROR_BUFFER_SIZE); /* NOLINT: memset(ex->message, 0, ERROR_BUFFER_SIZE) */
        exception_errors->message_count = 0;
    }
}

static u64 error_type(void) {
    if (exception_errors->message_count > 0) {
        return exception_errors->type[exception_errors->message_count - 1];
    }
    return ID_ERROR_NO_ERROR;
}

static const char* error_get(void) {
    if (exception_errors->message_count > 0) {
        return &exception_errors->message[(exception_errors->message_count - 1) * ERROR_MESSAGE_SIZE];
    }
    return NULL_PTR;
}

static FILE* error_std_vm_out(void) {
    return stdout;
}

static FILE* error_std_vm_err(void) {
    return stderr;
}

static u64 error_next(void) {
    if (exception_errors->message_count > 0) {
        exception_errors->message_count--;
        return TRUE;
    }
    return FALSE;
}

static u64 error_count(void) {
    return exception_errors->message_count;
}

/* public */
const system_error_methods PRIVATE_API(system_error_methods_definitions) = {
    .std_vm_out = error_std_vm_out,
    .std_vm_err = error_std_vm_err,
    .output = error_output,
    .exception = error_throw,
    .clear = error_clear,
    .type = error_type,
    .get = error_get,
    .next = error_next,
    .count = error_count
};

const system_error_methods* PRIVATE_API(error) = &PRIVATE_API(system_error_methods_definitions);
const system_error_methods* CALL(error) {
    return PRIVATE_API(error);
}
