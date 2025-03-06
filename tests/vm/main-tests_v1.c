/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 12:25:57 AM GMT+3
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

#define USING_TESTS

#include "main-tests_v1.h"

// #define RX_ENABLE_EXTERNAL_LINKING
// #define RX_DISABLE_TEST_DISCOVERY

#include "system/info/info_v1.h"

#include "test.h"

#define DEFAULT_SIZE 0x100

typedef struct test_data {
    void* ptr;
}* TEST_DATA;

const char* commit = GIT_COMMIT_HASH;

static void INIT init() {
#ifdef GIT_COMMIT_HASH
    printf("version: v%s\n", CALL(system_info)->version);
    // Replace the timestamp below with your Unix timestamp
    time_t unix_timestamp = (time_t)CALL(system_info)->timestamp;

    // Convert Unix timestamp to a time structure
    struct tm timeinfo;
    localtime_r(&unix_timestamp, &timeinfo);
    // Format the time structure into a string
    char buffer[160];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    // Print the formatted string
    printf("timestamp: %s\n", buffer);
#endif
}

int main(int argc, char** argv) {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    TEST_RUN(memory_micro_result, _memory_micro_test_suite);
    TEST_RUN(list_micro_result, _list_micro_test_suite);
    TEST_RUN(vm_v1_result, _vm_v1_test_suite);
    TEST_RUN(pointer_v1_result, _pointer_test_suite);
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return memory_micro_result | list_micro_result | vm_v1_result | pointer_v1_result;
}
