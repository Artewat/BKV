// main.c
// Тест кнопки на PD^7
#include "fx2regs.h"

// --- Глобально: определяем sbit ---
sbit BUTTON = PD ^ 7;  // Правильно: PD ^ 7

// Прототипы функций
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);

// --- Задержки ---
void delay_us(unsigned int us) {
    while (us--) {
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
        __asm NOP __endasm;
    }
}

void delay_ms(unsigned int ms) {
    while (ms--) {
        delay_us(1000);
    }
}

// --- Главная ---
void main(void) {
    // Настройка портов
    OEB  = 0x00;        // PB — выход (светодиоды)
    OED  = 0x00;        // Все PD — входы
    OED &= ~(1 << 7);   // Убедимся: PD7 — вход (1<<7 = 0x80)

    // --- Главный цикл ---
    while (1) {
        if (BUTTON == 0) {
            // Кнопка нажата
            IOB = 0xFF;
            delay_ms(100);
            IOB = 0x00;
            delay_ms(100);
        } else {
            IOB = 0x55;
            delay_ms(200);
        }
    }
}