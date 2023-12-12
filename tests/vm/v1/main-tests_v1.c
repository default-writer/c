/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   13 December 2023 at 0:49:21 GMT+3
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

#include <bits/types/time_t.h>
#define RXP_DEBUG_TESTS

#include "main-tests_v1.h"
#include "std/macros.h"
#include "vm/v1/system/info_v1.h"
#include "vm/v1/vm_v1.h"

#include <stdio.h>
#include <time.h>

#define DEFAULT_SIZE 0x100

typedef struct test_data {
    void* ptr;
}* TEST_DATA;

const char* commit = GIT_COMMIT_HASH;

static void INIT init() {
#if defined(GIT_COMMIT_HASH)
    printf("version: v%s\n", info_v1->version);
    // Replace the timestamp below with your Unix timestamp
    time_t unix_timestamp = (time_t)info_v1->timestamp;

    // Convert Unix timestamp to a time structure
    struct tm* timeinfo;
    timeinfo = localtime(&unix_timestamp);

    // Format the time structure into a string
    char buffer[160];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Print the formatted string
    printf("timestamp: %s\n", buffer);
#endif
}

int main(int argc, char** argv) {
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    TEST_RUN(list_micro_result, list_micro_tests);
    TEST_RUN(vm_v1_result, vm_v1_tests);
    TEST_RUN(pointer_v1_result, pointer_tests);
#ifdef USE_MEMORY_DEBUG_INFO
#if defined(VM_GLOBAL_DEBUG_INFO)
    global_statistics();
#endif
#endif
    return list_micro_result | vm_v1_result | pointer_v1_result;
}
