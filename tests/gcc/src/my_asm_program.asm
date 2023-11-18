section .data
    originalFunction dq 0
    args             dq 0
section .text
    global f_closure
    global my_asm_function
    extern original_function
    extern wrapperFunction
    extern f

my_asm_function:
    sub rsp, 24
    push rdi
    push rsi
    call f
    call wrapperFunction
    pop rsi
    pop rdi
    add rsp, 24
    mov rax, rdi
    imul rax, rsi
    ret
f_closure:
    push rbp      ; Save the base pointer
    mov rbp, rsp  ; Set up the new base pointer    ; Save the originalFunction and args in the data section
    mov rsp, rbp  ; Restore the stack pointer
    pop rbp       ; Restore the base pointer
    ret
