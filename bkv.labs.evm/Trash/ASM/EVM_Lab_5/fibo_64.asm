%macro PRINT_UDEC 2
    %%start:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    mov r10, buf_end
    mov byte [r10], 10
    dec r10
    mov rbx, 10
%%convert:
    xor rdx, rdx
    mov rax, %2
    div rbx
    mov %2, rax
    add dl, '0'
    mov [r10], dl
    dec r10
    test %2, %2
    jnz %%convert
    inc r10
    mov rsi, r10
    mov rdx, buf_end
    sub rdx, r10
    inc rdx
    mov rax, 1
    mov rdi, 1
    syscall
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

section .text
global _start

_start:
    mov rax, 1
    mov rbx, 1

ischo:
    ; Выводим текущее число
    PRINT_UDEC 8, rax

    xor rcx, rcx
    add rcx, rax
    add rcx, rbx          ; RCX = следующее число Фибоначчи

    ; Проверяем переполнение 64 бит
    cmp rcx, rax
    jb stop               ; переполнение — выходим

    mov rax, rbx
    mov rbx, rcx
    jmp ischo

stop:
    ; Выводим последнее число, которое поместилось
    PRINT_UDEC 8, rax

    ; sys_exit
    mov rax, 60
    xor rdi, rdi
    syscall

section .bss
    buf_res resb 24
    buf_end equ $
