section .text
global _start

; Подпрограмма OUTEAX — вывод беззнакового 32-битного числа из EAX
; Использует функцию 4 прерывания 80h (sys_write)
OUTEAX:
    pusha
    mov edi, buf_end      ; указатель на конец буфера
    mov byte [edi], 10    ; символ новой строки
    dec edi
    mov ebx, 10           ; делитель

.convert_loop:
    xor edx, edx
    div ebx               ; EAX = EAX / 10, EDX = остаток
    add dl, '0'
    mov [edi], dl
    dec edi
    test eax, eax
    jnz .convert_loop

    inc edi               ; EDI -> начало строки
    mov eax, 4            ; sys_write
    mov ebx, 1            ; stdout
    mov ecx, edi          ; буфер
    mov edx, buf_end
    sub edx, edi
    inc edx               ; включая '\n'
    int 80h
    popa
    ret

_start:
    mov esi, fibo
    mov eax, 1
    mov ebx, 1

ischo:
    xor ecx, ecx
    add ecx, eax
    add ecx, ebx          ; ECX = следующее число Фибоначчи

    ; Проверяем переполнение 32 бит (результат должен быть >= обоих слагаемых)
    cmp ecx, eax
    jb stop               ; переполнение — выходим

    mov [esi], eax        ; записываем в массив
    add esi, 4

    ; Выводим записанное число
    push esi
    mov eax, [esi - 4]
    call OUTEAX
    pop esi

    mov eax, ebx
    mov ebx, ecx
    jmp ischo

stop:
    ; Записываем последнее число, которое поместилось
    mov [esi], eax
    add esi, 4

    ; Выводим последнее число
    mov eax, [esi - 4]
    call OUTEAX

    ; sys_exit
    mov eax, 1
    xor ebx, ebx
    int 80h

section .data
    newline db 10
    fibo times 100 dd 0

section .bss
    buf_res resb 24
    buf_end equ $
