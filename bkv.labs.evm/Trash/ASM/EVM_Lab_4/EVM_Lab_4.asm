; Программа считывает значение из порта 0 (клавиши),
; переводит в base-ичную систему счисления и выводит
; на 7-сегментные индикаторы (порты 90h-9Eh)

org 100h                                ; точка входа программы

; Таблица кодов для 7-сегментного индикатора (0-9, A-F)
seg_0: db 3Fh                           ; код для 0
seg_1: db 06h                           ; код для 1
seg_2: db 5Bh                           ; код для 2
seg_3: db 4Fh                           ; код для 3
seg_4: db 66h                           ; код для 4
seg_5: db 6Dh                           ; код для 5
seg_6: db 7Dh                           ; код для 6
seg_7: db 07h                           ; код для 7
seg_8: db 7Fh                           ; код для 8
seg_9: db 6Fh                           ; код для 9
seg_A: db 77h                           ; код для A (10)
seg_B: db 7Ch                           ; код для B (11)
seg_C: db 39h                           ; код для C (12)
seg_D: db 5Eh                           ; код для D (13)
seg_E: db 79h                           ; код для E (14)
seg_F: db 71h                           ; код для F (15)

base dw 10                              ; система счисления (2-16)
digit0 dw 0                             ; младшая цифра
digit1 dw 0
digit2 dw 0
digit3 dw 0                             ; старшая цифра

main_loop:
        in al, 0                        ; читаем порт 0 (клавиши)
        mov ah, 0                       ; обнуляем старший байт
        mov cx, ax                      ; сохраняем значение в CX

        call convert_to_base            ; конвертируем в base-ичную систему
        call display_numbers            ; выводим на индикаторы

        jmp main_loop                   ; бесконечный цикл

; Процедура конвертации AX в систему счисления base
; Результат записывается в digit0-digit3
convert_to_base:
        push bx
        push dx
        push si

        ; Очищаем буфер цифр
        mov word [digit0], 0
        mov word [digit1], 0
        mov word [digit2], 0
        mov word [digit3], 0

        mov bx, word [base]              ; BX = основание системы счисления
        mov si, 0                       ; счётчик позиций

        mov cx, 0                       ; CX = текущая позиция
        mov dx, 0                       ; очищаем DX

div_loop:
        div bx                          ; AX / BX, остаток в DX
        cmp si, 0
        je .store0
        cmp si, 1
        je .store1
        cmp si, 2
        je .store2
        cmp si, 3
        je .store3
        jmp div_next
.store0:
        mov word [digit0], dx
        jmp div_next
.store1:
        mov word [digit1], dx
        jmp div_next
.store2:
        mov word [digit2], dx
        jmp div_next
.store3:
        mov word [digit3], dx

div_next:
        mov dx, 0                       ; очищаем DX для следующего деления
        add si, 1                       ; следующая позиция
        cmp si, 4                       ; все 4 цифры?
        jge div_end
        cmp ax, 0                       ; AX ещё не ноль?
        jne div_loop

div_end:
        pop si
        pop dx
        pop bx
        ret

; Получить код сегмента по цифре в AX, результат в AL
get_seg_code:
        push bx
        cmp ax, 0
        je .c0
        cmp ax, 1
        je .c1
        cmp ax, 2
        je .c2
        cmp ax, 3
        je .c3
        cmp ax, 4
        je .c4
        cmp ax, 5
        je .c5
        cmp ax, 6
        je .c6
        cmp ax, 7
        je .c7
        cmp ax, 8
        je .c8
        cmp ax, 9
        je .c9
        cmp ax, 10
        je .ca
        cmp ax, 11
        je .cb
        cmp ax, 12
        je .cc
        cmp ax, 13
        je .cd
        cmp ax, 14
        je .ce
        ; по умолчанию — F (15)
        mov al, [seg_F]
        jmp .done
.ce:
        mov al, [seg_E]
        jmp .done
.cd:
        mov al, [seg_D]
        jmp .done
.cc:
        mov al, [seg_C]
        jmp .done
.cb:
        mov al, [seg_B]
        jmp .done
.ca:
        mov al, [seg_A]
        jmp .done
.c9:
        mov al, [seg_9]
        jmp .done
.c8:
        mov al, [seg_8]
        jmp .done
.c7:
        mov al, [seg_7]
        jmp .done
.c6:
        mov al, [seg_6]
        jmp .done
.c5:
        mov al, [seg_5]
        jmp .done
.c4:
        mov al, [seg_4]
        jmp .done
.c3:
        mov al, [seg_3]
        jmp .done
.c2:
        mov al, [seg_2]
        jmp .done
.c1:
        mov al, [seg_1]
        jmp .done
.c0:
        mov al, [seg_0]
.done:
        pop bx
        ret

; Процедура вывода цифр на 7-сегментные индикаторы
display_numbers:
        push ax
        push bx
        push dx
        push si

        ; Старший индикатор - digit3
        mov dx, 9Eh
        mov ax, word [digit3]
        call get_seg_code
        out dx, al

        ; digit2
        mov dx, 9Ch
        mov ax, word [digit2]
        call get_seg_code
        out dx, al

        ; digit1
        mov dx, 9Ah
        mov ax, word [digit1]
        call get_seg_code
        out dx, al

        ; digit0 (младший)
        mov dx, 98h
        mov ax, word [digit0]
        call get_seg_code
        out dx, al

        pop si
        pop dx
        pop bx
        pop ax
        ret
