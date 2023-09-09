#include <stdio.h>
#include <stdlib.h>

typedef struct func_struct *func_struct_pointer, func_struct;
typedef void (*function)(func_struct_pointer self);

struct func_struct {
    function func;
};

void func_done(func_struct_pointer self) {
    printf("done %016llx\n", (unsigned long long)self);
}

int main(void) {
    func_struct_pointer main_struct_ptr = calloc(1, sizeof(func_struct));
    main_struct_ptr->func = func_done;
    main_struct_ptr->func(main_struct_ptr);
    free(main_struct_ptr);
    return 0;
}