#include <stdio.h>

typedef struct func_struct* func_struct_pointer;

void func_done(func_struct_pointer ptr) {
    printf("done %016llx\n", (unsigned long long)ptr);
}

struct func_struct {};

int main(void) {
    struct func_struct main_struct;
    func_done(&main_struct);
    return 0;
}