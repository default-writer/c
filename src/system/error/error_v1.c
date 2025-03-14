/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 6:00:44 AM GMT+3
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

static const char* error_messages[] = {
    [ID_ERROR_NO_ERROR] = "no error",
    [ID_ERROR_VM_NOT_INITIALIZED] = "vm not initialized",
    [ID_ERROR_POINTER_NOT_INITIALIZED] = "pointer not initialized",
    [ID_ERROR_ADDRESS_NOT_INITIALIZED] = "address not initialized",
    [ID_ERROR_ARGUMENT_VALUE_NOT_INITIALIZED] = "argument value not initialized",
    [ID_ERROR_INVALID_CONDITION] = "invalid condition"
};

#ifdef USE_MEMORY_DEBUG_INFO
static void error_stderr(enum error_message_code id, const char* func, const char* args, const char* file, int line);
#else
static void error_stderr(enum error_message_code id);
#endif

#ifdef USE_MEMORY_DEBUG_INFO
static void error_stderr(enum error_message_code id, const char* func, const char* args, const char* file, int line) {
#ifdef USE_TTY
    if (isatty(STDERR_FILENO)) {
        const char* start = "\x1b[31m";
        const char* end = "\x1b[0m";
        fprintf(stderr, "[%sERROR%s] %s: %s(%s): (%s:%d)\n", start, end, error_messages[id], func, args, file, line); /* NOLINT: fprintf(stderr) */
    }
#else
    fprintf(stderr, "[ERROR] %s: %s(%s) (%s:%d)\n", error_messages[id], func, args, file, line); /* NOLINT: fprintf(stderr) */
#endif
}
#else
static void error_stderr(enum error_message_code id) {
#ifdef USE_TTY
    if (isatty(STDERR_FILENO)) {
        const char* start = "\x1b[31m";
        const char* end = "\x1b[0m";
        fprintf(stderr, "[%sERROR%s] %s\n", start, end, error_messages[id]); /* NOLINT: fprintf(stderr) */
    }
#else
    fprintf(stderr, "[ERROR] %s\n", error_messages[id]); /* NOLINT: fprintf(stderr) */
#endif
}
#endif

/* api */
const error_api_type PRIVATE_API(error_api_methods_definitions) = {
    .stderr = &error_stderr,
};

const error_api_type* error_api = &PRIVATE_API(error_api_methods_definitions);
