section .text
global _start
_start:
	mov esi,fibo
	mov eax, 1
	mov ebx, 1
ischo:	xor ecx,ECX   ; обнуляем регистр
	add ecx,eax
	add ecx,ebx    ; вычислим следующее число Фиб.
	js a
	mov [esi], eax  ; запишем его в массив
	inc esi
	inc esi        ; установим указатель на след. элемент массива
	mov eax, ebx
	mov ebx, ecx
	jmp ischo
a:	mov [esi], eax  ; запишем  в массив
	inc esi
	inc esi        ; установим указатель на след. элемент массива
        mov [esi], ebx
        inc esi
        inc esi
        mov [esi], ecx
        mov eax,1            ; Системный вызов exit (sys_exit)
        mov ebx,0            ; Возвращаемое значение 0 (no error)
        int 80h

section .data

fibo times 100 dd 0

