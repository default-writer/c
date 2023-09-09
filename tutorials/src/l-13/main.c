#include <stdio.h>

void func_done(int a) {
    printf("done\n");
}

struct func_struct {};

int main(void) {
    int a = 42;
    func_done(a);
    return 0;
}