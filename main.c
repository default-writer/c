#include <stdio.h>
#include <stdlib.h>

struct qtype;
typedef struct q_type* q_type_ptr;

struct q_type {
    q_type_ptr next;
    q_type_ptr prev;
};

int main() {
    q_type_ptr q_ptr = (q_type_ptr)calloc(1, sizeof(q_type));
    if(q_ptr == NULL || &q_ptr == NULL)
    {
        printf("Error! memory not allocated.");
        exit(0);
    }
}