
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

uint32_t DJB2_hash(const uint8_t* str) {
    uint32_t hash = 5381;
    uint8_t c;
    while ((c = *str++))
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

uint32_t MurmurOAAT_32(const char* str, uint32_t h) {
    /* One-byte-at-a-time hash based on Murmur's mix */
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

uint32_t KR_v2_hash(const char* s) {
    /* Source: https://stackoverflow.com/a/45641002/5407270 */
    uint32_t hashval = 0;
    for (hashval = 0; *s != '\0'; s++)
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

uint32_t crc32b(const uint8_t* str) {
    /* Source: https://stackoverflow.com/a/21001712 */
    unsigned int byte, crc, mask;
    int i = 0, j;
    crc = 0xFFFFFFFF;
    while (str[i] != 0) {
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

uint32_t Coffin_hash(char const* input) {
    /* Source: https://stackoverflow.com/a/7666668/5407270 */
    uint32_t result = 0x55555555;
    while (*input) {
        result ^= *input++;
        result = _rotl32(result, 5);
    }
    return result;
}

uint32_t x17(const void* key, int len, uint32_t h) {
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    const uint8_t* data = (const uint8_t*)key;
    for (int i = 0; i < len; ++i) {
        h = 17 * h + (data[i] - ' ');
    }
    return h ^ (h >> 16);
}

/* hash: form hash value for string s */
uint32_t default_hash(char* source) {
    /* One-byte-at-a-time hash based on Murmur's mix */
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    uint32_t data = 0;
    if (source != 0) {
        uint32_t hash = ~0x5a32b847;
        char* ptr = source;
        while (*ptr != 0) {
            hash ^= *ptr;
            hash *= 0x5bd1e995;
            hash ^= hash >> 15;
            ptr++;
        }
        data = hash;
    }
    return data;
}

uint32_t apply_hash(int hash, char* line) {
    switch (hash) {
    case 1:
        return crc32b((const uint8_t*)line);
    case 2:
        return MurmurOAAT_32(line, SEED);
    case 3:
        return FNV(line, (uint32_t)strlen(line), SEED);
    case 4:
        return Jenkins_one_at_a_time_hash(line, strlen(line));
    case 5:
        return DJB2_hash((const uint8_t*)line);
    case 6:
        return KR_v2_hash(line);
    case 7:
        return Coffin_hash(line);
    case 8:
        return x17(line, (uint32_t)strlen(line), SEED);

    case 9:
        return default_hash((char*)line);
    default:
        break;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return 0;
    }
    /* Read arguments */
    const int hash_choice = atoi(argv[1]);
    const char* fn = argv[2];

    /* Read file */
    FILE* f = fopen(fn, "r");

    /* Read file line by line, calculate hash */
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0'; /* strip newline */
        uint32_t hash = apply_hash(hash_choice, line);
        printf("%08x\n", hash);
    }
    fclose(f);

    return 0;
}

#pragma GCC diagnostic pop
