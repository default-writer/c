#include <stdio.h>

struct func_struct {
};

void f_done(void* ptr) {
    printf("done in %016llx\n", (unsigned long long)ptr);
}

int main(void) {
    f_done(main);
    return 0;
}