#include <stdio.h>

struct func_struct {};

void func_done(void* ptr) {
    printf("done %016llx\n", (unsigned long long)ptr);
}

int main(void) {
    struct func_struct a;
    func_done(&a);
    return 0;
}