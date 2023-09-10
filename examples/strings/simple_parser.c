#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*f)(char** c);

f _;

void _a(char** c);
void _b(char** c);
void _c(char** c);

int result = 0;

/*
    a -> (b
    b -> )c
    c -> a
    c -> \0.
*/

void _a(char** c) {
    if (**c == '(') {
        _ = _b;
        (*c)++;
    } else {
        _ = 0;
    }
}

void _b(char** c) {
    if (**c == ')') {
        _ = _c;
        (*c)++;
    } else {
        _ = 0;
    }
}

void _c(char** c) {
    if (**c == '\0') {
        result = 1;
        _ = 0;
    } else {
        _ = _a;
    }
}

char* input = 0;

static void read_input() {
    memset(input, 0, 101);
    printf(">");
    char ch = 0;
    for (int i = 0; i < 100; i++) {
        ch = (char)getchar();
        if (ch == EOF || ch == '\n') {
            break;
        }
        input[i] = ch;
    }
}

int main(void) {
    char buffer[101]; 
    input = &buffer[0];
    memset(input, 0, 101);
    while (1) {
        read_input();
        if (*input == 'q' || *input == 0) {
            break;
        }
        _ = _a;
        while(_ != 0) {
            _(&input);
        }
        if (*input != '\0') {
            result = 0;
        }
        printf("%d\n",result);
        result = 0;
    }
    return 0;
}