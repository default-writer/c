#include <stdio.h>

typedef void* void_pointer;

void func_done(void_pointer ptr) {
    printf("done %016llx\n", (unsigned long long)ptr);
}

struct func_struct {};

int main(void) {
    struct func_struct main_struct;
    func_done(&main_struct);
    return 0;
}