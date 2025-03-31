/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 31, 2025 at 4:42:51 PM GMT+3
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

#include "system/os/os_v1.h"

#define ERROR_BUFFER_SIZE 4096

/* private */
static const char* error_messages[] = {
    [ID_ERROR_NO_ERROR] = "no error",
    [ID_ERROR_VM_NOT_INITIALIZED] = "vm not initialized",
    [ID_ERROR_INVALID_POINTER] = "invalid pointer",
    [ID_ERROR_INVALID_ARGUMENT] = "invalid argument",
    [ID_ERROR_INVALID_TYPE_ID] = "invalid type id",
    [ID_ERROR_INVALID_VALUE] = "invalid value",
};

static exception_type exception;
static exception_ptr ex = &exception;

static void error_output(FILE* output, u64 id, const char* message, u64 size);
static void error_throw(u64 id, const char* message, u64 size);
static void error_clear(void);
static u64 error_has(void);
static FILE* error_stdout(void);
static FILE* error_stderr(void);
static const char* error_get(void);

/* implementation */
static void error_output(FILE* output, u64 id, const char* message, u64 size) {
#ifdef USE_TTY
    if (isatty(STDERR_FILENO)) {
        const char* start = "\x1b[31m";
        const char* end = "\x1b[0m";
        fprintf(output, "%s[debug]%s %s: %s\n", start, end, error_messages[id], message); /* NOLINT: fprintf(output) */
    }
#else
    fprintf(output, "[debug] %s: %s\n", error_messages[id], message); /* NOLINT: fprintf(output) */
#endif
}

static void error_throw(u64 id, const char* message, u64 size) {
    ex->type = id;
    if (size < ERROR_BUFFER_SIZE) {
        CALL(os)->memcpy(ex->message, message, size); /* NOLINT: memcpy(ex->message */
        CALL(os)->memset(((u8*)ex->message) + size, 0x00, ERROR_BUFFER_SIZE - size);
    } else {
        CALL(os)->memcpy((u8*)ex->message, message, ERROR_BUFFER_SIZE - 1); /* NOLINT: memcpy(ex->message */
        ex->message[ERROR_BUFFER_SIZE - 1] = 0;
    }
}

static void error_clear(void) {
    CALL(os)->memset((u8*)ex->message, 0, 4096); /* NOLINT: memset(ex->message, 0, 4096) */
    ex->type = ID_ERROR_NO_ERROR;
}

static u64 error_has(void) {
    return ex->type == ID_ERROR_NO_ERROR ? FALSE : TRUE;
}

static FILE* error_stdout(void) {
    return stdout;
}

static FILE* error_stderr(void) {
    return stderr;
}

static const char* error_get(void) {
    return ex->message;
}

/* public */
const system_error_methods PRIVATE_API(system_error_methods_definitions) = {
    .output = error_output,
    .throw = error_throw,
    .clear = error_clear,
    .has = error_has,
    .stdout = error_stdout,
    .stderr = error_stderr,
    .get = error_get
};

const system_error_methods* PRIVATE_API(error) = &PRIVATE_API(system_error_methods_definitions);
const system_error_methods* CALL(error) {
    return PRIVATE_API(error);
}
