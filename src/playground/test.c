#include "std/common.h"

static char itoa(u8 v) { return (char)(v < 10 ? '0' + v : v<16 ? 'a' + (v - 10) : '0'); }

static void dec_to_hex(u8 value, char* const lo, char* const hi) {
    *lo = itoa(value % 16);
    *hi = itoa(value / 16);
}

static int rgb(u8 r, u8 g, u8 b, char* const output) {
    dec_to_hex(b % 256, &output[5], &output[4]);
    dec_to_hex(g % 256, &output[3], &output[2]);
    dec_to_hex(r % 256, &output[1], &output[0]);
    return 0;
}

int main() {
    char buffer[7] = { ' ', ' ', ' ', ' ', ' ', ' ', '\0' };
    rgb(12, 33, 18, buffer);
    printf("rgb(12,33,18) = #%s\n", buffer);
    return 0;
}
