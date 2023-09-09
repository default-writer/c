#include <stdio.h>

void func_done(int* a) {
    printf("done %d\n", *a);
}

struct func_struct {};

int main(void) {
    int a = 42;
    func_done(&a);
    return 0;
}