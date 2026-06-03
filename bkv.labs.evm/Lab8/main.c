#include <stdio.h>
#include <stdlib.h>

// Объявление внешней ассемблерной функции
extern double discr(double x, double v, double alpha);

// Вспомогательная функция для чтения одного числа double из файла
double read_double_from_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    double val;
    if (fscanf(f, "%lf", &val) != 1) {
        fprintf(stderr, "Ошибка: не удалось прочитать число из %s\n", filename);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    
    fclose(f);
    return val;
}

int main() {
    // 1. Считываем исходные данные из файлов
    double x = read_double_from_file("A.dat");
    double v = read_double_from_file("B.dat");
    double alpha = read_double_from_file("C.dat");

    // 2. Вызываем ассемблерную функцию для расчета траектории
    double y = discr(x, v, alpha);

    // 3. Вывод результата
    printf("Параметры полета:\n");
    printf("  Горизонтальная координата x = %.4f м\n", x);
    printf("  Начальная скорость v        = %.4f м/с\n", v);
    printf("  Угол бросания alpha         = %.4f рад\n", alpha);
    printf("----------------------------------------\n");
    printf("Высота снаряда на координате x: y = %.6f м\n", y);

    return 0;
}