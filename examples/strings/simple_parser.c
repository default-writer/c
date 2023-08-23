#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char c_1 = '(';
char c_2 = '(';

typedef void (*f)(char** c);

f _;

void _a(char** c);
void _b(char** c);
void _c(char** c);
void _q(char** c);

int result = 0;

/*

    a -> (b
    b -> )c
    c -> \0_
      -> a

*/

void _a(char** c) {
    if (**c == '(') {
        _ = _b;
        (*c)++;
    } else {
        _ = _q;
    }
}

void _b(char** c) {
    if (**c == ')') {
        _ = _c;
        (*c)++;
    } else {
        _ = _q;
    }
}

void _c(char** c) {
    if (**c++ == '\0') {
        result = 1;
        _ = _q;
    } else {
        _ = _a;
    }
}

void _q(char **c) {
    _ = 0;
}

char* input = 0;

static char* read_input() {
    memset(input, 0, 100);
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
    input = calloc(1, 100);
    while (1) {
        read_input();
        if (*input == 'q') {
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