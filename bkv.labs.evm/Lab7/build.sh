#!/bin/bash

echo "Компиляция ассемблерного кода..."
nasm -f elf64 sici.asm -o sici.o

echo "Компиляция C-кода и линковка..."
gcc -no-pie -o main main.c sici.o -lm

if [ $? -eq 0 ]; then
    echo "Успешная сборка! Запуск программы:"
    echo "=================================================="
    ./main
else
    echo "Ошибка компиляции или линковки."
fi