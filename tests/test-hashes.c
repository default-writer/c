/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 2048
#define SEED 0x12345678

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "rexo/include/rexo.h"

void get_hash_from_file(int hash_id);

void get_data();
void get_hash(int hash_id);

struct hash_functions {
};

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    struct hash_functions* ctx;
}* TEST_DATA;

RX_SET_UP(test_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
}

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* test init */
RX_TEST_CASE(tests, test_hash_0, .fixture = test_fixture) {
    get_hash(0);
}

/* test init */
RX_TEST_CASE(tests, test_hash_1, .fixture = test_fixture) {
    get_hash(1);
}

/* test init */
RX_TEST_CASE(tests, test_hash_2, .fixture = test_fixture) {
    get_hash(2);
}

/* test init */
RX_TEST_CASE(tests, test_hash_3, .fixture = test_fixture) {
    get_hash(3);
}

/* test init */
RX_TEST_CASE(tests, test_hash_4, .fixture = test_fixture) {
    get_hash(4);
}

/* test init */
RX_TEST_CASE(tests, test_hash_5, .fixture = test_fixture) {
    get_hash(5);
}

/* test init */
RX_TEST_CASE(tests, test_hash_6, .fixture = test_fixture) {
    get_hash(6);
}

/* test init */
RX_TEST_CASE(tests, test_hash_7, .fixture = test_fixture) {
    get_hash(7);
}

/* test init */
RX_TEST_CASE(tests, test_hash_8, .fixture = test_fixture) {
    get_hash(8);
}

/* test init */
RX_TEST_CASE(tests, test_hash_9, .fixture = test_fixture) {
    get_hash(9);
}

uint32_t DJB2_hash(const uint8_t* str, unsigned long size) {
    uint32_t hash = 5381;
    uint8_t c;
    while (--size > 0 && (c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

uint32_t FNV(const void* key, int len, uint32_t h) {
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    h ^= 2166136261UL;
    const uint8_t* data = (const uint8_t*)key;
    for (int i = 0; i < len; i++) {
        h ^= data[i];
        h *= 16777619;
    }
    return h;
}

uint32_t MurmurOAAT_32(const char* str, unsigned long size, uint32_t h) {
    /* One-byte-at-a-time hash based on Murmur's mix */
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; --size > 0 && *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

uint32_t KR_v2_hash(const char* s, unsigned long size) {
    /* Source: https://stackoverflow.com/a/45641002/5407270 */
    uint32_t hashval = 0;
    for (hashval = 0; --size > 0 && *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval;
}
uint32_t Jenkins_one_at_a_time_hash(const char* str, size_t len) {
    uint32_t hash, i;
    for (hash = i = 0; i < len; ++i) {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

uint32_t crc32b(const uint8_t* str, unsigned long size) {
    /* Source: https://stackoverflow.com/a/21001712 */
    unsigned int byte, crc, mask;
    int i = 0, j;
    crc = 0xffffffff;
    while (--size > 0 && str[i] != 0) {
        byte = str[i];
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--) {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
}

inline uint32_t _rotl32(uint32_t x, int32_t bits) {
    return x << bits | x >> (32 - bits); /* C idiom: will be optimized to a single operation */
}

uint32_t Coffin_hash(char const* input, unsigned long size) {
    /* Source: https://stackoverflow.com/a/7666668/5407270 */
    uint32_t result = 0x55555555;
    while (--size > 0 && *input) {
        result ^= *input++;
        result = _rotl32(result, 5);
    }
    return result;
}

uint32_t x17(const void* key, unsigned long size, uint32_t h) {
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    const uint8_t* data = (const uint8_t*)key;
    for (unsigned long i = 0; i < size; ++i) {
        h = 17 * h + (data[i] - ' ');
    }
    return h ^ (h >> 16);
}

/* hash: form hash value for string s */
uint32_t default_hash(char* source, unsigned long size) {
    /* One-byte-at-a-time hash based on Murmur's mix */
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    uint32_t data = 0;
    if (source != 0) {
        uint32_t hash = ~0x5a32b847;
        char* ptr = source;
        while (--size > 0 && *ptr != 0) {
            hash ^= *ptr;
            hash *= 0x5bd1e995;
            hash ^= hash >> 15;
            ptr++;
        }
        data = hash;
    }
    return data;
}

uint32_t apply_hash(int hash, char* line, unsigned long size) {
    switch (hash) {
    case 1:
        return crc32b((const uint8_t*)line, size);
    case 2:
        return MurmurOAAT_32(line, size, SEED);
    case 3:
        return FNV(line, (uint32_t)size, SEED);
    case 4:
        return Jenkins_one_at_a_time_hash(line, size);
    case 5:
        return DJB2_hash((const uint8_t*)line, size);
    case 6:
        return KR_v2_hash(line, size);
    case 7:
        return Coffin_hash(line, size);
    case 8:
        return x17(line, (uint32_t)size, SEED);
    case 9:
        return default_hash((char*)line, size);
    default:
        break;
    }
    return 0;
}

struct list {
    char* data;
    unsigned long size;
    struct list* next;
};

static struct list* list;

void get_data() {
    const char* fn = "all_english_words.txt";

    /* Read file */
    FILE* f = fopen(fn, "r");

    /* Read file line by line, calculate hash */
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0'; /* strip newline */
        struct list* current = calloc(1, sizeof(struct list));
        unsigned long size = ((strlen(line) >> 3) | 1) << 3;
        current->data = calloc(1, size);
        current->size = size;
        memcpy(current->data, line, size);
        current->next = list;
        list = current;
    }
    fclose(f);
}

void get_hash(int hash_id) {
    struct list* current = list;

    while (current != 0) {
        char* data = current->data;
        uint32_t hash = apply_hash(hash_id, data, current->size);
        printf("%08x\n", hash);
        current = current->next;
    }
}

int main(int argc, char* argv[]) {

    get_data();

    /* Execute the main function that runs the test cases found. */
    int result = rx_run(0, NULL) == RX_SUCCESS ? 0 : 1;

    struct list* current = list;

    while (current != 0) {
        struct list* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    return result;
}

#pragma GCC diagnostic pop
