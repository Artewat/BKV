#include "fx2regs.h"

// Явное объявление регистров Cypress FX2LP в области XDATA
__xdata __at 0xE601 volatile unsigned char MY_IFCFG;   // Конфигурация интерфейса
__xdata __at 0xE604 volatile unsigned char MY_OED;     // Направление Порта D
__xdata __at 0xE607 volatile unsigned char MY_PINSD;   // Физический статус пинов Порта D

void delay_ms(unsigned int ms) {
    unsigned int i;
    while(ms--) {
        for(i = 0; i < 600; i++) {
            __asm NOP __endasm;
        }
    }
}

void main(void) {
    // 1. Полностью отключаем аппаратный движок GPIF/FIFO, переводим порты в GPIO
    MY_IFCFG &= ~0x03; 

    // 2. Кнопки: Настраиваем пины PD0 и PD1 строго на ВВОД
    MY_OED &= ~0x03; 

    // 3. Светодиоды: Весь Порт B на вывод
    OEB = 0xFF; 
    IOB = 0x00; 

    // Стартовый маркер (первые 4 диода горят 1 секунду)
    IOB = 0x0F; 
    delay_ms(1000); 

    while(1) {
        // Проверяем КНОПКУ 1 (на пине PD0)
        // Если кнопка нажата, нулевой бит регистра MY_PINSD станет равен 0
        if ((MY_PINSD & 0x01) == 0) {
            IOB = 0xAA; // Зажигаем светодиоды через один (10101010)
            while ((MY_PINSD & 0x01) == 0); // Ждем, пока кнопку ОТПУСТЯТ
            delay_ms(50); // Защита от дребезга
        }

        // Проверяем КНОПКУ 2 (на пине PD1)
        // Если кнопка нажата, первый бит регистра MY_PINSD станет равен 0
        else if ((MY_PINSD & 0x02) == 0) {
            IOB = 0x55; // Зажигаем инверсный вариант (01010101)
            while ((MY_PINSD & 0x02) == 0); // Ждем, пока кнопку ОТПУСТЯТ
            delay_ms(50); // Защита от дребезга
        }

        // Если ни одна кнопка не нажата, горит наш дежурный байт 0x33
        else {
            IOB = 0x33; 
        }
    }
}