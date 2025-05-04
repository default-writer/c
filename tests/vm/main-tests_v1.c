/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 4, 2025 at 5:18:48 AM GMT+3
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

#define USING_MAIN_TESTS
#include "main-tests_v1.h"

#define USING_API
#include "system/os/os_v1.h"

#include "system/info/info_v1.h"

#define USING_TESTS
#include "test.h"

#include <time.h>

typedef struct test_data {
    void_ptr ptr;
}* TEST_DATA;

const char* commit = GIT_COMMIT_HASH;

static void init() {
#ifdef GIT_COMMIT_HASH
    printf("version: v%s\n", CALL(info)->version);
    time_t unix_timestamp = (time_t)CALL(info)->timestamp;
    struct tm timeinfo;
#if defined(_WIN32)
    localtime_s(&timeinfo, &unix_timestamp);
#else
    localtime_r(&unix_timestamp, &timeinfo);
#endif
    char buffer[160];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    printf("timestamp: %s\n", buffer);
#endif
}

#ifdef USE_MEMORY_DEBUG_INFO
int main(int argc, const char** argv) {
    init();
    init_statistics();
    TEST_RUN(PRIVATE_API(tests_memory_test_suite));
    TEST_RUN(PRIVATE_API(tests_list_test_suite));
    TEST_RUN(PRIVATE_API(tests_vm_test_suite));
    TEST_RUN(PRIVATE_API(tests_pointer_suite));
    TEST_RUN(PRIVATE_API(tests_hashtable_test_suite));
    TEST_RUN(PRIVATE_API(tests_api_test_suite));
    TEST_RUN(PRIVATE_API(tests_api_v2_test_suite));
    TEST_RUN(PRIVATE_API(tests_error_test_suite));
    result_statistics();
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
#else
int main(int argc, const char** argv) {
    init();
    TEST_RUN(PRIVATE_API(tests_memory_test_suite));
    TEST_RUN(PRIVATE_API(tests_list_test_suite));
    TEST_RUN(PRIVATE_API(tests_vm_test_suite));
    TEST_RUN(PRIVATE_API(tests_pointer_suite));
    TEST_RUN(PRIVATE_API(tests_hashtable_test_suite));
    TEST_RUN(PRIVATE_API(tests_api_test_suite));
    TEST_RUN(PRIVATE_API(tests_api_v2_test_suite));
    TEST_RUN(PRIVATE_API(tests_error_test_suite));
    return rx_main(0, NULL, argc, argv) == RX_SUCCESS ? 0 : 1;
}
#endif
