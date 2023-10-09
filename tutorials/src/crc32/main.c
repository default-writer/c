#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t crc32b(const uint8_t* str, unsigned long size) {
    /* Source: https://stackoverflow.com/a/21001712 */
    unsigned int byte, mask, i = 0, crc = 0xffffffff;
    while (--size > 0) {
        byte = str[i];
        crc = crc ^ byte;
        for (unsigned int j = 8; j > 0; j--) {
            mask = ~(crc & 1) + 1;
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
}

int main(void) {
    const char* str = "zyzzyvas\n";
    uint32_t result = crc32b((const uint8_t*)str, strlen(str));
    printf("CRC32: 0x%08x\n", result);
    return 0;
}
