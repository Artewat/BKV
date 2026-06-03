global discr
default rel

section .data
    const_g dq 9.81
    const_2 dq 2.0

section .text
; double discr(double x, double v, double alpha)
discr:
    sub rsp, 40

    ; Минимизация обращений к памяти: читаем аргументы из XMM в стек ОДИН раз
    movsd [rsp], xmm0     ; [rsp] = x
    movsd [rsp+8], xmm1   ; [rsp+8] = v
    movsd [rsp+16], xmm2  ; [rsp+16] = alpha

    ; ==========================================================
    ; ВЫЧИСЛЕНИЯ ИСКЛЮЧИТЕЛЬНО В СТЕКЕ FPU
    ; Формула: y = x * tan(alpha) - (g * x^2) / (2 * v^2 * cos^2(alpha))
    ; ==========================================================

    ; 1. Тригонометрия
    fld qword [rsp+16]    ; st0 = alpha
    fsincos               ; st0 = cos(alpha), st1 = sin(alpha)

    ; 2. Знаменатель: denom = 2 * v^2 * cos^2(alpha)
    fld st0               ; st0 = cos, st1 = cos, st2 = sin
    fmul st0, st0         ; st0 = cos^2, st1 = cos, st2 = sin
    fld qword [rsp+8]     ; st0 = v, st1 = cos^2, st2 = cos, st3 = sin
    fmul st0, st0         ; st0 = v^2, st1 = cos^2, st2 = cos, st3 = sin
    fmulp st1, st0        ; st0 = v^2 * cos^2, st1 = cos, st2 = sin (pop v^2)
    fmul qword [const_2]  ; st0 = 2 * v^2 * cos^2 [denom], st1 = cos, st2 = sin

    ; 3. Числитель: numer = g * x^2
    fld qword [const_g]   ; st0 = g, st1 = denom, st2 = cos, st3 = sin
    fld qword [rsp]       ; st0 = x, st1 = g, st2 = denom, st3 = cos, st4 = sin
    fmul st0, st0         ; st0 = x^2, st1 = g, st2 = denom, st3 = cos, st4 = sin
    fmulp st1, st0        ; st0 = g * x^2 [numer], st1 = denom, st2 = cos, st3 = sin (pop g)

    ; 4. Вычисление term2 = numer / denom
    ; fdivrp st(i), st(0) выполняет: st(i) = st(0) / st(i), затем pop st(0)
    ; Здесь st0=numer, st1=denom. Результат: st1 = numer / denom
    fdivrp st1, st0       ; st1 = term2, pop st0.
                          ; Стек: st0 = term2, st1 = cos, st2 = sin

    ; 5. Вычисление term1 = x * sin(alpha) / cos(alpha)
    fld qword [rsp]       ; st0 = x, st1 = term2, st2 = cos, st3 = sin
    fmul st0, st3         ; st0 = x * sin, st1 = term2, st2 = cos, st3 = sin
    fdiv st0, st2         ; st0 = (x * sin) / cos [term1], st1 = term2, st2 = cos, st3 = sin

    ; 6. Финальное вычисление: y = term1 - term2
    ; Сейчас st0 = term1, st1 = term2. Нам нужно term1 - term2.
    fxch st1              ; st0 = term2, st1 = term1, st2 = cos, st3 = sin
    ; fsubp st(i), st(0) выполняет: st(i) = st(i) - st(0), затем pop st(0)
    ; Здесь st0=term2, st1=term1. Результат: st1 = term1 - term2
    fsubp st1, st0        ; st1 = result, pop st0.
                          ; Стек: st0 = result, st1 = cos, st2 = sin

    ; 7. Очистка стека FPU (нам нужно оставить только result в st0)
    fxch st2              ; st0 = sin, st1 = cos, st2 = result
    fstp st0              ; pop sin. Стек: st0 = cos, st1 = result
    fxch st1              ; st0 = result, st1 = cos
    fstp st1              ; st1 = result, pop st0. Стек: st0 = result

    ; ==========================================================
    ; Возврат результата в C-программу
    ; ==========================================================
    fstp qword [rsp+24]   ; Сохраняем result из st0 в память, стек FPU становится ПУСТЫМ (глубина 0)
    movsd xmm0, qword [rsp+24] ; Помещаем результат в xmm0 для возврата по ABI
    
    add rsp, 40
    ret