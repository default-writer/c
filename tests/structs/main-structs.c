#include "std/common.h"

typedef struct A1 {
    u64 counter;
}* A;

int main(void) {
    A a = { 0 };
    a = calloc(1, sizeof(struct A1));
    a->counter = 1;
    free(a);
    return 0;
}
