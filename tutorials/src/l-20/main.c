#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"

#include <stdio.h>

typedef struct func_struct* func_struct_pointer;

struct func_struct { };

void func_done(func_struct_pointer ptr) {
    printf("done %016llx\n", (unsigned long long)ptr);
}

int main(void) {
    struct func_struct main_struct;
    func_done(&main_struct);
    return 0;
}
