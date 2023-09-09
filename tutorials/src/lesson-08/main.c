#include <stdio.h>
#include <stdlib.h>

typedef struct func_struct *func_struct_pointer, func_struct;
typedef void (*function)(func_struct_pointer self);

struct func_struct {
    function func;
};

void func_done(func_struct_pointer self) {
    printf("done in %016llx\n", (unsigned long long)self);
}

int main(void) {
    func_struct_pointer ptr = calloc(1, sizeof(func_struct));
    ptr->func = func_done;
    ptr->func(ptr);
    free(ptr);
    return 0;
}