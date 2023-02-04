#include "std/common.h"

static char itoa(u8 v) {
    return (char)(v < 10 ? '0' + v : (v < 16 ? 'a' + (v - 10) : '0'));
}

static void dec_to_hex(u8 value, char* lo, char* hi) {
    *lo = itoa(value & 0x0F);
    *hi = itoa((u8)(value & 0xF0 >> 4));
}

static int rgb(int r, int g, int b, char* output) {
    dec_to_hex((u8)b, &output[5], &output[4]);
    dec_to_hex((u8)g, &output[3], &output[2]);
    dec_to_hex((u8)r, &output[1], &output[0]);
    return 0;
}

int main() {
    char buffer[7] = { ' ', ' ', ' ', ' ', ' ', ' ', '\0' };
    rgb(12, 33, 18, buffer);
    printf("rgb(12,33,18) = #%s\n", buffer);
    return 0;
}
