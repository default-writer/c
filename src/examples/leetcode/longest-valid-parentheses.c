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

int process(state_machine_pointer p) {
    if (*p->current_position == open_normal_bracket) {
        p->fire_position = p->current_position + 1;
        return 1;
    }
    if (*p->current_position == close_normal_bracket) {
        while (p->fire_position > p->start_position && *(p->fire_position - 1) == '+') {
            p->fire_position--;
        }
        if (p->fire_position > p->start_position) {
            if (*(p->fire_position - 1) == open_normal_bracket && *p->current_position == close_normal_bracket) {
                *(p->fire_position-- - 1) = '+'; /* burn it! */
                *p->current_position = '+'; /* burn it! */
            }
        }
        return 1;
    }
    return 0;
}

int longestValidParentheses(char* ptr) {
    state_machine_pointer p = 0;
    p = (state_machine_pointer)calloc(1, sizeof(state_machine));
    unsigned long size = strlen(ptr);
    if (size == 0) {
        return 0;
    }
    p->current_position = ptr;
    p->fire_position = ptr;
    p->start_position = ptr;
    while (*p->current_position != 0 && process(p)) {
        p->current_position++;
    }
    p->current_position = ptr;
    int counter = 0;
    int max_counter = 0;
    while (*p->current_position != 0) {
        while (*p->current_position != 0 && *p->current_position != '+')
            p->current_position++;
        while (*p->current_position != 0 && *p->current_position == '+') {
            counter++;
            p->current_position++;
        }
        if (max_counter < counter) {
            max_counter = counter;
        }
        counter = 0;
    }
    int result = max_counter;
    free(p);
    return result;
}

const char* input[] = {
    "(()())",
    "())",
    "()))",
    "())))",
    "()))))",
    "())))))",
    "()))(())",
    "(()(((()",
    ")()())",
    "()(())",
    "())()",
    ")()())()()(",
    "()()()",
    "())((())",
    "(())()",
    "()(()",
    "((()",
    ")(",
    "()",
    ")(()))",
    "((())(",
    "()())",
    "(())",
    "((((()))()))",
    "(())((()((()))",
    "(())(((((()))",
    "(())(((()",
    "(())",
    "(((()",
    "(((((",
    "())))))",
    "))))))))",
    "((()))",
    "(())((())",
    "(())",
    "(())()()()",
    "(())((()))",
    "(())((())))",
    ")",
    "())",
    "())((()",
    "(())((()",
    ")()(",
    "()(()))"
};

int main(int argc, char** argv) {
    int len = sizeof(input) / sizeof(char*);
    int i = 0;
    for (; i < len; i++) {
        char* ptr = calloc(1, strlen(input[i]) + 2);
        memcpy(ptr + 1, input[i], strlen(input[i]));
        int result = longestValidParentheses(ptr + 1);
        printf(" %s => %d\n", input[i], result);
        free(ptr);
    }
    return 0;
}
