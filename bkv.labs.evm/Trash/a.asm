org 100h
mov ax, 1234h
mov bx, 1111H
add ax, bx
mov si, b
mov [si], ax
a: jmp a
b dw 0