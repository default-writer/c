#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct func_struct *func_struct_pointer, func_struct;
typedef void (*function)(func_struct_pointer self);

static void _a(func_struct_pointer self);
static void _b(func_struct_pointer self);
static void _c(func_struct_pointer self);

struct func_struct {
    char* input;
    int result;
    function _;
    const function _a;
    const function _b;
    const function _c;
};

static func_struct finite_automata = {
    .result = 0,
    ._ = 0,
    ._a = _a,
    ._b = _b,
    ._c = _c
};

static void _a(func_struct_pointer self) {
    if (*self->input == '(') {
        self->_ = self->_b;
        self->input++;
    } else {
        self->_ = 0;
    }
}

static void _b(func_struct_pointer self) {
    if (*self->input == ')') {
        self->_ = self->_c;
        self->input++;
    } else {
        self->_ = 0;
    }
}

static void _c(func_struct_pointer self) {
    if (*self->input == '\0') {
        self->result = 1;
        self->_ = 0;
    } else {
        self->_ = self->_a;
    }
}

static void read_input(func_struct_pointer self) {
    memset(self->input, 0, 101);
    printf(">");
    char ch = 0;
    for (int i = 0; i < 100; i++) {
        ch = (char)getchar();
        if (ch == EOF || ch == '\n') {
            break;
        }
        self->input[i] = ch;
    }
}

int main(void) {
    func_struct_pointer ptr = &finite_automata;
    char* buffer = calloc(1, 101);
    while (1) {
        ptr->input = buffer;
        read_input(ptr);
        if (*ptr->input == 'q') {
            break;
        }
        ptr->_ = _a;
        while(ptr->_ != 0) {
            ptr->_(&finite_automata);
        }
        if (*ptr->input != '\0') {
            ptr->result = 0;
        }
        printf("%d\n", ptr->result);
        ptr->result = 0;
    }
    free(buffer);
    return 0;
}