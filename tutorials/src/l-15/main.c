#include <stdio.h>

struct func_struct {};

void func_done(int* a) {
    printf("done %d\n", *a);
}

int main(void) {
    int a = 42;
    func_done(&a);
    return 0;
}