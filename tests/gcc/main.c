#include <stdio.h>

static void myNakedFunction(void) __attribute__((naked));

static void myFunction(void) {
    char* message;
    
    // Inline assembly to load the pointer from a user-defined register
    __asm__ (
        "movq %%rdi, %0" : "=r" (message)
    );
    
    printf("%s\n", message);
}

static void myNakedFunction(void) {
    __asm__ (
        "   pushq %rbp\n"
        "   movq %rsp, %rbp"
    );
    __asm__ (
        "   call myFunction"
    );
    __asm__ (
        "   nop\n"
        "   popq %rbp\n"
        "   ret"
    );
}

int main(void) {
    const char* message = "Hello, world!";

    // Load the address of the character string into rdi
    __asm__ (
        "movq %0, %%rdi" : : "r" (message)
    );

    myNakedFunction();
    return 0;
}
