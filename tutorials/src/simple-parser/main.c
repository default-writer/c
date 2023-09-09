#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct func_struct *func_struct_pointer, func_struct;
typedef void (*function)(func_struct_pointer self);

static void _a(func_struct_pointer self);
static void _b(func_struct_pointer self);
static void _c(func_struct_pointer self);
static void _d(func_struct_pointer self);

struct func_struct {
    char* input;
    int result;
    function _;
    const function _a;
    const function _b;
    const function _c;
    const function _d;
};

static func_struct finite_automata = {
    .input = 0,
    .result = 0,
    ._ = 0,
    ._a = _a,
    ._b = _b,
    ._c = _c,
    ._d = _d
};

static void _a(func_struct_pointer self) {
    if (*self->input == '(') {
        self->_ = self->_b;
        self->input++;
    } else {
        self->_ = self->_d;
    }
}

static void _b(func_struct_pointer self) {
    if (*self->input == ')') {
        self->_ = self->_c;
        self->input++;
    } else {
        self->_ = self->_d;
    }
}

static void _c(func_struct_pointer self) {
    if (*self->input == '\0') {
        self->result = 1;
        self->_ = self->_d;
    } else {
        self->_ = self->_a;
    }
}

static void _d(func_struct_pointer self) {
    self->_ = 0;
}

static void read_input(func_struct_pointer self) {
    memset(self->input, 0, 100);
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
    char* buffer = calloc(1, 100);
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