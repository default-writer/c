section .text
global my_asm_function
my_asm_function:
    ; Inputs: 
    ;   - rdi: First argument (int value)
    ;   - rsi: Second argument (int multiplier)
    ; Outputs:
    ;   - rax: Result of the multiplication
    ; Clobbered registers: None

    ; Multiply the first argument by the second argument
    mov rax, rdi
    imul rax, rsi
    ret
