#include <stdio.h>

struct func_struct {};

void func_done(void* ptr) {
    printf("done %016llx\n", (unsigned long long)ptr);
}

int main(void) {
    int a = 42;
    func_done(&a);
    return 0;
}