global compute_sici
extern log

default rel

section .data
    gamma     dq 0.57721566490153286060651209  ; Постоянная Эйлера-Маскерони
    eps       dq 1.0e-15                       ; Точность остановки ряда
    one       dq 1.0
    neg_one   dq -1.0
    neg_inf   dq 0xFFF0000000000000            ; IEEE 754 представление -Infinity
    saved_x   dq 0.0

section .text
; Сигнатура: void compute_sici(double x, double *out_si, double *out_ci)
; System V AMD64 ABI:
; xmm0 = x           (1-й аргумент, float)
; rdi  = out_si      (2-й аргумент, указатель)
; rsi  = out_ci      (3-й аргумент, указатель)

compute_sici:
    ; Проверка на x == 0.0
    pxor xmm15, xmm15
    ucomisd xmm0, xmm15
    je .handle_zero
    
    ; === КРИТИЧЕСКИ ВАЖНО: Сохраняем указатели, так как call log их затрет ===
    push rdi
    push rsi
    
    ; Сохраняем x, так как вызов log затрет xmm0
    movsd [saved_x], xmm0
    
    ; Вызываем log(x) из libc
    ; Стек: при входе rsp % 16 == 8. 
    ; push rdi -> rsp % 16 == 0
    ; push rsi -> rsp % 16 == 8
    ; sub rsp, 8 -> rsp % 16 == 0 (Идеальное 16-байтное выравнивание для call)
    sub rsp, 8
    call log
    add rsp, 8
    
    ; === Восстанавливаем указатели в исходные регистры ===
    pop rsi
    pop rdi
    
    ; xmm0 теперь содержит log(x)
    movsd xmm5, [gamma]
    addsd xmm5, xmm0          ; xmm5 = SumCi = gamma + log(x)
    
    ; Восстанавливаем x для вычислений
    movsd xmm0, [saved_x]
    
    ; x2 = x * x
    movsd xmm1, xmm0
    mulsd xmm1, xmm1          ; xmm1 = x^2

    ; Инициализация для Si(x): Z = x, SumSi = x
    movsd xmm2, xmm0          ; Z
    movsd xmm3, xmm0          ; SumSi

    ; Инициализация для Ci(x): W = 1.0
    movsd xmm4, [one]         ; W
    
    mov rcx, 1                ; Счетчик цикла k = 1

.loop:
    ; Вычисляем знаменатели: 2k, 2k+1, 2k-1
    mov rax, rcx
    shl rax, 1                ; rax = 2k
    mov r9, rax               ; r9 = 2k
    mov r10, rax
    inc r10                   ; r10 = 2k + 1
    dec r9                    ; r9 = 2k - 1
    
    cvtsi2sd xmm6, r10        ; xmm6 = (double)(2k + 1)
    cvtsi2sd xmm7, r9         ; xmm7 = (double)(2k - 1)
    cvtsi2sd xmm8, rax        ; xmm8 = (double)(2k)
    
    ; Denom_Z = (2k) * (2k + 1)
    movsd xmm9, xmm8
    mulsd xmm9, xmm6
    
    ; Denom_W = (2k - 1) * (2k)
    movsd xmm10, xmm7
    mulsd xmm10, xmm8
    
    ; M = -x^2
    movsd xmm11, [neg_one]
    mulsd xmm11, xmm1         ; xmm11 = -x^2
    
    ; M_Z = -x^2 / Denom_Z
    movsd xmm12, xmm11
    divsd xmm12, xmm9
    
    ; M_W = -x^2 / Denom_W
    movsd xmm13, xmm11
    divsd xmm13, xmm10
    
    ; Z = Z * M_Z
    movsd xmm14, xmm2
    mulsd xmm14, xmm12        ; xmm14 = Z_new
    
    ; W = W * M_W
    movsd xmm15, xmm4
    mulsd xmm15, xmm13        ; xmm15 = W_new
    
    ; Обновляем Z и W
    movsd xmm2, xmm14         ; Z = Z_new
    movsd xmm4, xmm15         ; W = W_new
    
    ; T = Z_new / (2k + 1)
    divsd xmm14, xmm6         ; xmm14 = T
    
    ; V = W_new / (2k)
    divsd xmm15, xmm8         ; xmm15 = V
    
    ; SumSi += T
    addsd xmm3, xmm14
    
    ; SumCi += V
    addsd xmm5, xmm15
    
    ; Проверка на максимальное количество итераций
    cmp rcx, 60
    jge .end_loop
    
    ; Проверка сходимости: |T| < eps
    movq rax, xmm14
    btr rax, 63               ; Сбрасываем 63-й бит (знак), получая модуль числа
    movq xmm14, rax
    ucomisd xmm14, [eps]
    jae .continue_loop        ; Если |T| >= eps, продолжаем
    
    ; Проверка сходимости: |V| < eps
    movq rax, xmm15
    btr rax, 63
    movq xmm15, rax
    ucomisd xmm15, [eps]
    jae .continue_loop        ; Если |V| >= eps, продолжаем
    
    ; Если оба члена ряда меньше eps, завершаем цикл
    jmp .end_loop

.continue_loop:
    inc rcx
    jmp .loop

.end_loop:
    ; Запись результатов по сохраненным указателям
    movsd [rdi], xmm3         ; *out_si = SumSi
    movsd [rsi], xmm5         ; *out_ci = SumCi
    ret

.handle_zero:
    ; Сюда мы попадаем ДО вызова log, поэтому rdi и rsi еще целы
    pxor xmm0, xmm0               ; Si(0) = 0.0
    movq xmm1, [neg_inf]          ; Ci(0) = -inf
    movsd [rdi], xmm0             ; *out_si = 0.0
    movsd [rsi], xmm1             ; *out_ci = -inf
    ret