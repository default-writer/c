#include <stdio.h>

void func_done(int* a) {
    printf("done %016llx\n", (unsigned long long)a);
}

struct func_struct {};

int main(void) {
    int a = 42;
    func_done(&a);
    return 0;
}