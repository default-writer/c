#include <stdio.h>

void func_done(void* ptr) {
    printf("done in %016llx\n", (unsigned long long)ptr);
}

int main(void) {
    func_done(main);
    return 0;
}