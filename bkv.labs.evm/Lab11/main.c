#include "fx2regs.h" // Ищет в текущей папке

void delay_us(unsigned int us) {
    while(us--) {
        __asm NOP __endasm; // Специфичный для SDCC синтаксис ассемблера
        __asm NOP __endasm;
    }
}

void delay_ms(unsigned int ms) {
    while(ms--) delay_us(1000);
}

void Init_Peripherals(void) {
    OEB = 0xFF; // Порт B на вывод (LED)
    OED = 0xFF; // Порт D на вывод (Centronics Data)
    OEC |= 0x01; // PC0 на вывод (STROBE)
    PC0 = 1;    // Изначально STROBE в пассивном высоком состоянии
}

void Centronics_SendByte(unsigned char byte_to_send) {
    IOD = byte_to_send;
    delay_us(2); 
    PC0 = 0; // Строб вниз
    delay_us(5); 
    PC0 = 1; // Строб вверх
    delay_us(2); 
}

void Update_Snake(void) {
    // Начальная позиция: 3 горящих диода в самом начале (0000 0111) -> 0x07
    static unsigned char snake = 0x07; 
    
    // Направление движения: 0 - влево (к старшим битам), 1 - вправо (к младшим битам)
    static unsigned char direction = 0; 

    // Выводим текущее положение змейки на светодиоды Порта B
    IOB = snake;

    if (direction == 0) {
        // Двигаем змейку влево (к PB7)
        snake <<= 1;
        
        // Проверяем, уперлась ли голова змейки в левый край (PB7)
        // Если маска достигла 1110 0000 (0xE0), значит дальше двигаться нельзя
        if (snake == 0xE0) { 
            direction = 1; // Меняем направление на обратное
        }
    } else {
        // Двигаем змейку вправо (назад к PB0)
        snake >>= 1;
        
        // Проверяем, вернулся ли хвост на правый край (PB0)
        // Если маска снова стала 0000 0111 (0x07), значит приползли обратно
        if (snake == 0x07) { 
            direction = 0; // Меняем направление на движение влево
        }
    }
}

void main(void) {
    Init_Peripherals();
    while(1) {
        Centronics_SendByte(0xAA);
        Update_Snake();
        delay_ms(150); 
    }
}