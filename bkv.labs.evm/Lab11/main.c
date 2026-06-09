// main.c
// Проект для Cypress CY7C68013A — режим SPP (Centronics) + змейка на LED
// Собирается с использованием SDCC

#include "fx2regs.h"

// ===================================================
// ПРОТОТИПЫ ФУНКЦИЙ (во избежание implicit declaration)
// ===================================================
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
void Init_Peripherals(void);
void Centronics_SendByte(unsigned char byte_to_send);
void Update_Snake(void);

// ===================================================
// ЗАДЕРЖКИ
// ===================================================
void delay_us(unsigned int us) {
    while (us--) {
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        // ~8 NOP = ~0.67 мкс при 12 MHz CPU → приближенно 1 мкс
        // Точность не критична для SPP
    }
}

void delay_ms(unsigned int ms) {
    while (ms--) {
        delay_us(1000);  // 1000 × ~1 мкс = ~1 мс
    }
}

// ===================================================
// ИНИЦИАЛИЗАЦИЯ ПЕРИФЕРИИ
// ===================================================
void Init_Peripherals(void) {
    OEB  = 0xFF;        // Порт B — выход (светодиоды)
    OED  = 0xFF;        // Порт D — выход (данные Centronics)
    OEC |= 0x01;        // PC0 — выход (STROBE)
    PC0 = 1;            // Изначально строб неактивен (высокий)
}

// ===================================================
// ОТПРАВКА БАЙТА ПО SPP (Centronics)
// ===================================================
void Centronics_SendByte(unsigned char byte_to_send) {
    IOD = byte_to_send;     // Устанавливаем данные на порт D
    delay_us(2);            // Setup time

    PC0 = 0;                // Строб: активный ноль
    delay_us(5);            // Пульс строба (больше 0.5 мкс)

    PC0 = 1;                // Завершаем строб
    delay_us(2);            // Hold time

    // Выводим тот же байт на светодиоды (порт B)
    IOB = byte_to_send;
}

// ===================================================
// АНИМАЦИЯ ЗМЕЙКИ НА СВЕТОДИОДАХ
// ===================================================
void Update_Snake(void) {
    static unsigned char snake = 0x07;      // Начало: 3 диода (0000_0111)
    static unsigned char direction = 0;     // 0 — влево, 1 — вправо

    IOB = snake;  // Выводим на светодиоды

    if (direction == 0) {
        snake <<= 1;                    // Двигаем влево
        if (snake == 0xE0) {           // Достигли головой PB7 (1110_0000)
            direction = 1;
        }
    } else {
        snake >>= 1;                    // Двигаем вправо
        if (snake == 0x07) {           // Вернулись к началу
            direction = 0;
        }
    }
}

// ===================================================
// ГЛАВНЫЙ ЦИКЛ
// ===================================================
void main(void) {
    Init_Peripherals();

    while (1) {
        Centronics_SendByte(0xAA);    // Отправляем байт
        delay_ms(150);                // Пауза

        Update_Snake();               // Анимируем змейку
        delay_ms(150);                // Пауза
    }
}