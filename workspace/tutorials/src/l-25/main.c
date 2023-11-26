#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"

#include <stdio.h>

typedef struct func_struct* func_struct_pointer;
typedef void (*function)(func_struct_pointer self);

struct func_struct {
    function func;
};

void func_done(func_struct_pointer ptr) {
    printf("done %016llx\n", (unsigned long long)ptr->func);
}

int main(void) {
    struct func_struct main_struct;
    main_struct.func = func_done;
    function func = func_done;
    func(&main_struct);
    return 0;
}
