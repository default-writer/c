#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct state_machine {
    char* current_position;
    char* fire_position;
    char* start_position;
} *state_machine_pointer, state_machine;

const char open_normal_bracket = '(';
const char close_normal_bracket = ')';
const char open_figure_bracket = '{';
const char close_figure_bracket = '}';
const char open_square_bracket = '[';
const char close_square_bracket = ']';

int process(state_machine_pointer p) {
    if (*p->current_position == open_normal_bracket) {
        *p->fire_position++ = *p->current_position;
        return 1;
    }
    if (*p->current_position == open_figure_bracket) {
        *p->fire_position++ = *p->current_position;
        return 1;
    }
    if (*p->current_position == open_square_bracket) {
        *p->fire_position++ = *p->current_position;
        return 1;
    }
    if (*p->current_position == close_normal_bracket) {
        if (p->fire_position > p->start_position) {
            if (*(p->fire_position - 1) == open_normal_bracket) {
                *p->fire_position-- = 0; /* burn it! */
                return 1;
            }
        }
        return 0;
    }
    if (*p->current_position == close_figure_bracket) {
        if (p->fire_position > p->start_position) {
            if (*(p->fire_position - 1) == open_figure_bracket) {
                *p->fire_position-- = 0; /* burn it! */
                return 1;
            }
        }
        return 0;
    }
    if (*p->current_position == close_square_bracket) {
        if (p->fire_position > p->start_position) {
            if (*(p->fire_position - 1) == open_square_bracket) {
                *p->fire_position-- = 0; /* burn it! */
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

int isValid(char* s) {
    state_machine_pointer p = 0;
    p = (state_machine_pointer)calloc(1, sizeof(state_machine));
    p->current_position = s;
    p->fire_position = s;
    p->start_position = s;
    while (*p->current_position != 0 && process(p)) {
        p->current_position++;
    }
    int result = *p->current_position == 0 && *p->fire_position != 0 && p->fire_position == p->start_position;
    free(p);
    return result;
}

const char* input[] = {
    "[[[]",
    "()",
    "({})",
    "[([]])",
    "([[]])",
    "({})",
    "[{]}",
    "[{({{}})()}]",
    "{()}",
    "({})",
    "[([]])",
    "()",
    "({)}",
    "()[]{}",
    "({)}()[]{}",
    "{}",
    "[]",
    "]",
    "()}",
    "[({)}]",
    "{()}",
    "[{(}}]"
};

int main(int argc, char** argv) {
    int len = sizeof(input) / sizeof(char*);
    int i = 0;
    for (; i < len; i++) {
        char* ptr = calloc(1, strlen(input[i]) + 2);
        memcpy(ptr + 1, input[i], strlen(input[i]));
        int result = isValid(ptr + 1);
        printf(" %s => %s\n", input[i], result == 1 ? "true" : "false");
        free(ptr);
    }
    return 0;
}
