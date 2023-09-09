#include <stdio.h>

struct func_struct {};

void func_done(int a) {
    printf("done\n");
}

int main(void) {
    int a = 42;
    func_done(a);
    return 0;
}