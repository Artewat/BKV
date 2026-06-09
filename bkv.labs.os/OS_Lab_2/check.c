#include <stdio.h> // printf - вывод текста
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE - коды возврата
#include <fcntl.h> // open, fcntl - файловые операции
#include <unistd.h> // read, lseek, close - работа с файлами
#include <errno.h> // errno - код ошибки

int main(void) // главная функция
{
    int fd = open("places.txt", O_RDONLY); // открываем файл только для чтения
    if (fd == -1) // ошибка открытия
    { perror("Не удалось открыть places.txt"); return EXIT_FAILURE; }

    int total_seats = 0; // всего мест
    int file_free = 0; // свободно по файлу ('0')
    int file_occupied = 0; // занято по файлу ('1')
    int lock_free = 0; // свободно по блокировке
    int lock_occupied = 0; // занято по блокировке
    int can_book = 0; // доступно для бронирования
    int cannot_book = 0; // недоступно

    printf("\nОБЗОР МЕСТ В АВТОБУСЕ\n\n");
    printf("Ряд Место | Файл | Блок.    | PID      | Доступ\n");
    printf("----------|------|----------|----------|----------\n");
    
    for (int r = 1; r <= 7; r++) // перебор рядов (7 штук)
    {
        for (int p = 1; p <= 4; p++) // перебор мест в ряду (4 штуки)
        {
            total_seats++; // счётчик общего количества

            int entry_idx = (r - 1) * 4 + (p - 1); // индекс записи (0-27)
            off_t file_pos = 62 + entry_idx * 10; // 62 байта заголовок + позиция

            char f_status;
            lseek(fd, file_pos + 8, SEEK_SET); // +8 - смещение до поля статуса
            if (read(fd, &f_status, 1) != 1) // читаем значение
            { f_status = '?'; } // ошибка чтения - ставим '?'

            // статистика по файлу
            if (f_status == '0') { file_free++; }
            else if (f_status == '1') { file_occupied++; }

            // проверка блокировки
            struct flock lk; // структура блокировки
            lk.l_type = F_WRLCK; // тип - блокировка записи
            lk.l_whence = SEEK_SET; // от начала файла
            lk.l_start = file_pos; // начало - наша запись
            lk.l_len = 10; // длина 10 байт
            lk.l_pid = 0; // PID (заполнится F_GETLK)

            printf("  %d   %d   |   %c    | ", r, p, f_status);

            int lk_res = fcntl(fd, F_GETLK, &lk);
            if (lk_res == -1) // ошибка
            { printf("ОШИБКА   |     -    | "); printf("Н/Д\n"); }
            else if (lk.l_type == F_UNLCK) // блокировки нет
            {
                lock_free++; // счётчик свободных по блокировке
                printf("СВОБОДНО |     -    | ");
                if (f_status == '0') { can_book++; printf("ДА\n"); } // можно бронировать
                else // статус '1', но блокировки нет
                { cannot_book++; printf("НЕТ (помечено занятым, без блокировки)\n"); }
            }
            else // есть блокировка
            {
                lock_occupied++; // счётчик занятых по блокировке
                printf("ЗАНЯТО   |   %-7d| ", lk.l_pid);
                cannot_book++; // недоступно
                printf("НЕТ (блокировка PID=%d)\n", lk.l_pid);
            }
        }
    }
    
    printf("---------------------------------------------------------------\n");
    printf("Файл: 0 - свободно, 1 - занято\n");
    printf("Доступ: ДА - можно бронировать, НЕТ - нельзя\n");
    printf("\nИТОГО: %d мест,\n", total_seats);
    printf("По файлу: свободно %d, занято %d\n", file_free, file_occupied);
    printf("По блокировкам: свободно %d, занято %d\n", lock_free, lock_occupied);
    printf("Готово к бронированию: %d\n", can_book);
    printf("Недоступно: %d\n", cannot_book);

    close(fd); // закрываем дескриптор
    return EXIT_SUCCESS; // успех
}
