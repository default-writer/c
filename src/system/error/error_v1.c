/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 17, 2025 at 11:05:49 AM GMT+3
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

#include "error_v1.h"

#define MAX_ERROR_COUNT 1024

static const char* error_messages[] = {
    [ID_ERROR_NO_ERROR] = "no error",
    [ID_ERROR_NO_MEMORY] = "no memory",
    [ID_ERROR_NULLPTR] = "0-ptr reference",
    [ID_ERROR_INVALID_ADDRESS] = "invalid address",
    [ID_ERROR_VM_NOT_INITIALIZED] = "vm not initialized",
    [ID_ERROR_POINTER_NOT_INITIALIZED] = "pointer not initialized",
    [ID_ERROR_ADDRESS_NOT_INITIALIZED] = "address not initialized",
    [ID_ERROR_ARGUMENT_VALUE_NOT_INITIALIZED] = "argument value not initialized",
    [ID_ERROR_INVALID_CONDITION] = "invalid condition"
};

typedef struct error_message {
    u64 code;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* func;
    const char* args;
    const char* file;
    int line;
#endif
} error_message_type;

static error_message_type error[MAX_ERROR_COUNT] = { 0 };
static u64 error_count = 0;

/* declaration */
static void error_clear(void);
static void error_get_stack_trace(void);
#ifdef USE_MEMORY_DEBUG_INFO
static void error_set_error_code(u64 error_code, const char* error_func, const char* error_args, const char* error_file, int error_line);
#else
static void error_set_error_code(u64 error_code);
#endif
static u64 error_get_error_code(void);

/* implementation */
static void error_clear(void) {
    error_count = 0;
    memset(&error, 0, MAX_ERROR_COUNT * sizeof(error_message_type)); /* NOLINT: MAX_ERROR_COUNT * sizeof(error_message_type) */
}

#ifdef USE_MEMORY_DEBUG_INFO
static void error_set_error_code(u64 error_code, const char* error_func, const char* error_args, const char* error_file, int error_line) {
    error[error_count] = (error_message_type) {
        .code = error_code,
        .func = error_func,
        .args = error_args,
        .file = error_file,
        .line = error_line,
    };
    if (error_count < MAX_ERROR_COUNT - 1) {
        ++error_count;
    }
}
static void error_get_stack_trace(void) {
    if (error_count == 0) {
        return;
    }
#ifdef USE_TTY
    if (isatty(STDERR_FILENO)) {
        const char* start = "\x1b[31m";
        const char* end = "\x1b[0m";
        for (int i = 0; i < error_count; i++) {
            fprintf(stderr, "[%sERROR%s] %s: %s(%s): (%s:%d)\n", start, end, error_messages[error[i].code], error[i].func, error[i].args, error[i].file, error[i].line); /* NOLINT: fprintf(stderr) */
        }
    }
#else
    for (u64 i = 0; i < error_count; i++) {
        fprintf(stderr, "[ERROR] %s: %s(%s) (%s:%d)\n", error_messages[error[i].code], error[i].func, error[i].args, error[i].file, error[i].line); /* NOLINT: fprintf(stderr) */
    }
    error_count = 0;
#endif
}
#else
static void error_set_error_code(u64 error_code) {
    error[error_count] = (error_message_type) {
        .code = error_code
    };
    if (error_count < MAX_ERROR_COUNT - 1) {
        ++error_count;
    }
}

static void error_get_stack_trace(void) {
    if (error_count == 0) {
        return;
    }
#ifdef USE_TTY
    if (isatty(STDERR_FILENO)) {
        const char* start = "\x1b[31m";
        const char* end = "\x1b[0m";
        for (int i = 0; i < error_count; i++) {
            fprintf(stderr, "[%sERROR%s] %s\n", start, end, error_messages[id]); /* NOLINT: fprintf(stderr) */
        }
    }
#else
    for (u64 i = 0; i < error_count; i++) {
        fprintf(stderr, "[ERROR] %s\n", error_messages[error[i].code]); /* NOLINT: fprintf(stderr) */
    }
#endif
    error_count = 0;
}
#endif

static u64 error_get_error_code(void) {
    return error[error_count].code;
}

/* api */
const error_api_type PRIVATE_API(error_api_methods_definitions) = {
    .clear = error_clear,
    .get_error_code = error_get_error_code,
    .set_error_code = error_set_error_code,
    .get_stack_trace = error_get_stack_trace
};

const error_api_type* error_api = &PRIVATE_API(error_api_methods_definitions);
