/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 11:18:54 AM GMT+3
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

#define USING_API
#include "system/os/os_v1.h"

#include "system/info/info_v1.h"

#define USING_TESTS
#include "test.h"

typedef struct test_data {
    void_ptr ptr;
}* TEST_DATA;

const char* commit = GIT_COMMIT_HASH;

static void init() {
#ifdef GIT_COMMIT_HASH
    printf("version: v%s\n", CALL(info)->version);
    time_t unix_timestamp = (time_t)CALL(info)->timestamp;
    struct tm timeinfo;
    localtime_r(&unix_timestamp, &timeinfo);
    char buffer[160];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    printf("timestamp: %s\n", buffer);
#endif
}

#ifdef USE_MEMORY_DEBUG_INFO
int main(int argc, const char** argv) {
    init();
    init_statistics();
    TEST_RUN(_tests_memory_test_suite);
    TEST_RUN(_tests_list_test_suite);
    TEST_RUN(_tests_vm_test_suite);
    TEST_RUN(_tests_pointer_suite);
    TEST_RUN(_tests_hashtable_test_suite);
    TEST_RUN(_tests_api_test_suite);
    result_statistics();
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
#else
int main(int argc, const char** argv) {
    init();
    TEST_RUN(_tests_memory_test_suite);
    TEST_RUN(_tests_list_test_suite);
    TEST_RUN(_tests_vm_test_suite);
    TEST_RUN(_tests_pointer_suite);
    TEST_RUN(_tests_hashtable_test_suite);
    TEST_RUN(_tests_api_test_suite);
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
#endif
