#include <stdio.h>

typedef struct func_struct *func_struct_pointer;
typedef void (*function)(func_struct_pointer self);

struct func_struct {};

void func_done(func_struct_pointer ptr) {
    printf("done %016llx\n", (unsigned long long)ptr);
}

int main(void) {
    struct func_struct main_struct;
    function func = func_done;
    func(&main_struct);
    return 0;
}