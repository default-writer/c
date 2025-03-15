/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 15, 2025 at 4:04:55 PM GMT+3
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

#define USING_MAIN_TESTS
#include "main-tests_v1.h"

#define USING_SYSTEM_API
#include "system/api/api_v1.h"

#include "system/info/info_v1.h"

#define USING_TESTS
#include "test.h"

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

#ifdef USE_MEMORY_DEBUG_INFO
int main(int argc, const char** argv) {
    init_statistics();
    TEST_RUN(_memory_micro_test_suite);
    TEST_RUN(_list_micro_test_suite);
    TEST_RUN(_vm_v1_test_suite);
    TEST_RUN(_pointer_test_suite);
    result_statistics();
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
#else
int main(int argc, const char** argv) {
    TEST_RUN(_memory_micro_test_suite);
    TEST_RUN(_list_micro_test_suite);
    TEST_RUN(_vm_v1_test_suite);
    TEST_RUN(_pointer_test_suite);
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
#endif
