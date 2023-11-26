#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"

#include <stdio.h>

struct func_struct { };

void func_done(int* a) {
    printf("done %016llx\n", (unsigned long long)a);
}

int main(void) {
    int a = 42;
    func_done(&a);
    return 0;
}
