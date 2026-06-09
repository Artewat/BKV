#include <stdio.h> // printf, perror - вывод сообщений и ошибок
#include <stdlib.h> // atoi, EXIT_SUCCESS, EXIT_FAILURE - преобразование строк и коды возврата
#include <string.h> // strcmp - сравнение строк
#include <fcntl.h> // open, O_RDWR, fcntl - работа с файлами и блокировками
#include <unistd.h> // read, write, lseek, close, getpid - файловые операции и PID
#include <errno.h> // errno - глобальная переменная ошибки
#include <signal.h> // signal, SIGINT - обработка сигналов

// Структура для хранения данных о занятых местах
typedef struct
{
    int is_active; // флаг активности блокировки (1/0)
    int seat_row; // номер ряда (1-7)
    int seat_num; // номер места (1-4)
    off_t file_offset; // позиция в файле (байты)
} SeatReservation;

SeatReservation reservations[28]; // таблица всех бронирований (макс. 28 мест)
int active_reservations = 0; // счётчик текущих бронирований
int file_descriptor; // дескриптор открытого файла places.txt (глобальный для обработчика)

void reset_all_seats() // восстановление начального состояния файла (замена '1' на '0')
{
    printf("\nСброс всех мест в исходное состояние...\n");

    for (int r = 1; r <= 7; r++) // перебор всех рядов (7 штук)
    {
        for (int p = 1; p <= 4; p++) // перебор всех мест в ряду (4 штуки)
        {
            int entry_idx = (r - 1) * 4 + (p - 1); // индекс записи (0-27)
            off_t file_pos = 62 + entry_idx * 10; // 62 байта заголовок + позиция записи

            char current_val;
            lseek(file_descriptor, file_pos + 8, SEEK_SET); // +8 - смещение до поля статуса
            read(file_descriptor, &current_val, 1); // считываем текущее значение

            if (current_val == '1') // если занято ('1'), меняем на свободно ('0')
            {
                lseek(file_descriptor, file_pos + 8, SEEK_SET);
                char reset_val = '0';
                write(file_descriptor, &reset_val, 1);
                printf("Место %d-%d очищено\n", r, p);
            }
        }
    }
    printf("Сброс завершён\n");
}

void release_all_locks() // снятие всех блокировок текущего процесса
{
    for (int idx = 0; idx < active_reservations; idx++)
    {
        struct flock unlock_req; // структура для операции разблокировки
        unlock_req.l_type = F_UNLCK; // F_UNLCK - команда снятия блокировки
        unlock_req.l_whence = SEEK_SET;
        unlock_req.l_start = reservations[idx].file_offset;
        unlock_req.l_len = 10;
        unlock_req.l_pid = 0;
        fcntl(file_descriptor, F_SETLK, &unlock_req); // выполняем разблокировку

        lseek(file_descriptor, reservations[idx].file_offset + 8, SEEK_SET);
        char free_mark = '0';
        write(file_descriptor, &free_mark, 1); // проставляем '0' в файле (освобождено)

        printf("Разблокировано место %d-%d\n", reservations[idx].seat_row, reservations[idx].seat_num);
    }
    active_reservations = 0; // обнуляем счётчик
}

void handle_sigint(int sig) // обработка прерывания по Ctrl+C (SIGINT)
{
    (void)sig; // подавляем предупреждение о неиспользуемом параметре
    printf("\nПолучен сигнал SIGINT (Ctrl+C)\n");

    if (active_reservations > 0) // есть активные блокировки
    { release_all_locks(); } // освобождаем все

    reset_all_seats(); // возвращаем файлу исходное состояние
    close(file_descriptor); // закрываем файловый дескриптор
    printf("Завершение работы программы\n");
    exit(EXIT_SUCCESS); // корректный выход
}

void show_commands() // вывод списка доступных команд
{
    printf("\nСписок команд:\n");
    printf("book <ряд> <место> - забронировать место\n");
    printf("cancel <ряд> <место> - отменить бронирование места\n");
    printf("cancel_all - отменить все бронирования\n");
    printf("info - отобразить информацию о текущих бронированиях\n");
    printf("verify <ряд> <место> - проверить статус конкретного места\n");
    printf("quit - выйти из программы (с восстановлением файла)\n");
    printf("commands - показать эту справку\n");
}

void verify_seat(int row, int place) // проверка статуса места (ряд и номер)
{
    int entry_idx = (row - 1) * 4 + (place - 1); // вычисляем индекс записи (0-27)
    off_t file_pos = 62 + entry_idx * 10; // 62 байта заголовок + позиция записи

    // считываем статус из файла (основной дескриптор)
    char status_val;
    lseek(file_descriptor, file_pos + 8, SEEK_SET); // +8 - смещение до поля статуса
    read(file_descriptor, &status_val, 1); // читаем значение (поле B, 1 байт)

    printf("\nМесто %d-%d:\n", row, place);
    printf("Значение в файле: %c\n", status_val); // '0' - свободно, '1' - занято

    // проверяем, забронировано ли это место нами (через массив reservations)
    int is_ours = 0;
    for (int k = 0; k < active_reservations; k++)
    { if (reservations[k].seat_row == row && reservations[k].seat_num == place) { is_ours = 1; break; } }
    if (is_ours) { printf("Статус: ЗАБРОНИРОВАНО вами (PID=%d)\n", getpid()); return; }

    // открываем отдельный дескриптор ТОЛЬКО для проверки блокировки другим процессом
    int verify_fd = open("places.txt", O_RDONLY);
    if (verify_fd == -1) { perror("Не удалось открыть файл для проверки"); return; }

    // пытаемся установить тестовую блокировку
    struct flock test_lock;
    test_lock.l_type = F_WRLCK;
    test_lock.l_whence = SEEK_SET;
    test_lock.l_start = file_pos;
    test_lock.l_len = 10;
    test_lock.l_pid = 0;

    // F_SETLK - если вернёт -1, значит место занято другим процессом
    int test_res = fcntl(verify_fd, F_SETLK, &test_lock);
    if (test_res == -1 && (errno == EAGAIN || errno == EACCES))
    {
        // не удалось - место занято другим процессом
        test_lock.l_type = F_WRLCK; // узнаём PID через F_GETLK
        if (fcntl(verify_fd, F_GETLK, &test_lock) != -1 && test_lock.l_pid != 0)
        { printf("Статус: ЗАБРОНИРОВАНО процессом PID=%d\n", test_lock.l_pid); }
        else { printf("Статус: ЗАНЯТО другим процессом (PID не определён)\n"); }
    }
    else // тестовая блокировка успешна - место свободно
    {
        printf("Статус: СВОБОДНО (никто не занимал)\n");
    }

    close(verify_fd); // закрываем вспомогательный дескриптор
}

void book_seat(int row, int place) // бронирование места (ряд и номер)
{
    if (row < 1 || row > 7 || place < 1 || place > 4) // валидация координат
    { printf("Ошибка: ряд от 1 до 7, место от 1 до 4\n"); return; }

    int entry_idx = (row - 1) * 4 + (place - 1); // индекс записи (0-27)
    off_t target_offset = 62 + entry_idx * 10; // 62 байта заголовок + позиция записи

    struct flock probe; // структура для проверки
    probe.l_type = F_WRLCK; // проверяем блокировку на запись
    probe.l_whence = SEEK_SET;
    probe.l_start = target_offset;
    probe.l_len = 10;
    probe.l_pid = 0;

    if (fcntl(file_descriptor, F_GETLK, &probe) == -1) // получаем информацию о блокировке
    { perror("Ошибка проверки блокировки"); return; }
    if (probe.l_type != F_UNLCK) // место уже занято
    { printf("Место %d-%d занято другим процессом (PID=%d)\n", row, place, probe.l_pid); return; }
    for (int k = 0; k < active_reservations; k++) // проверяем, не занято ли нами
    {
        if (reservations[k].seat_row == row && reservations[k].seat_num == place)
        { printf("Место %d-%d уже забронировано вами\n", row, place); return; }
    }

    struct flock new_lock; // структура для новой блокировки
    new_lock.l_type = F_WRLCK; // F_WRLCK - исключительная блокировка
    new_lock.l_whence = SEEK_SET;
    new_lock.l_start = target_offset;
    new_lock.l_len = 10;
    new_lock.l_pid = 0;

    int lock_res = fcntl(file_descriptor, F_SETLK, &new_lock); // устанавливаем блокировку
    if (lock_res == -1)
    {
        if (errno == EAGAIN || errno == EACCES)
        { printf("Место %d-%d уже занято (ошибка %d)\n", row, place, errno); }
        else { perror("Ошибка установки блокировки"); }
        return;
    }

    lseek(file_descriptor, target_offset + 8, SEEK_SET);
    char occupied_mark = '1';
    if (write(file_descriptor, &occupied_mark, 1) == -1) // ставим '1' (занято)
    {
        perror("Ошибка записи статуса");
        new_lock.l_type = F_UNLCK;
        fcntl(file_descriptor, F_SETLK, &new_lock); // откат блокировки
        return;
    }

    // сохраняем информацию о бронировании
    reservations[active_reservations].is_active = 1;
    reservations[active_reservations].seat_row = row;
    reservations[active_reservations].seat_num = place;
    reservations[active_reservations].file_offset = target_offset;
    active_reservations++;

    printf("\nМесто %d-%d успешно забронировано\n", row, place);
    printf("PID процесса: %d\n", getpid());
    printf("Активных бронирований: %d\n", active_reservations);
}

void cancel_seat(int row, int place) // отмена бронирования конкретного места
{
    int found_idx = -1; // индекс найденного бронирования
    for (int k = 0; k < active_reservations; k++) // поиск в массиве
    {
        if (reservations[k].seat_row == row && reservations[k].seat_num == place)
        { found_idx = k; break; }
    }
    if (found_idx == -1) // бронирование не найдено
    { printf("Место %d-%d не забронировано вами\n", row, place); return; }

    struct flock unlock_req; // структура для разблокировки
    unlock_req.l_type = F_UNLCK; // F_UNLCK - снятие блокировки
    unlock_req.l_whence = SEEK_SET;
    unlock_req.l_start = reservations[found_idx].file_offset;
    unlock_req.l_len = 10;
    unlock_req.l_pid = 0;

    if (fcntl(file_descriptor, F_SETLK, &unlock_req) == -1) // снимаем блокировку
    { perror("Ошибка при отмене бронирования"); return; }
    lseek(file_descriptor, reservations[found_idx].file_offset + 8, SEEK_SET);
    char free_mark = '0';
    if (write(file_descriptor, &free_mark, 1) == -1) // ставим '0' (свободно)
    { perror("Ошибка записи статуса"); return; }
    printf("Бронирование места %d-%d отменено\n", row, place);

    // удаляем запись из массива
    for (int k = found_idx; k < active_reservations - 1; k++)
    { reservations[k] = reservations[k + 1]; }
    active_reservations--;
}

void cancel_all_command() // отмена всех бронирований
{
    if (active_reservations == 0) // нет активных бронирований
    { printf("Нет активных бронирований для отмены\n"); return; }
    release_all_locks(); // вызов функции массовой отмены
}

void show_info() // отображение текущего статуса бронирований
{
    if (active_reservations > 0) // есть активные бронирования
    {
        printf("\nТекущие бронирования (%d):\n", active_reservations);
        for (int k = 0; k < active_reservations; k++)
        { printf("Место %d-%d\n", reservations[k].seat_row, reservations[k].seat_num); }
        printf("PID процесса: %d\n", getpid());
    }
    else { printf("\nНет активных бронирований\n"); }
}

int main(int argc, char *argv[]) // главная функция
{
    signal(SIGINT, handle_sigint); // регистрируем обработчик: Ctrl+C вызовет handle_sigint
    file_descriptor = open("places.txt", O_RDWR); // открываем файл мест для чтения/записи
    if (file_descriptor == -1) // ошибка открытия
    { perror("Не удалось открыть places.txt"); return EXIT_FAILURE; }

    printf("\nСИСТЕМА БРОНИРОВАНИЯ МЕСТ АВТОБУСА\n");
    printf("Программа удерживает блокировки до завершения.\n");
    printf("При выходе (quit или Ctrl+C) файл places.txt будет восстановлен.\n");
    printf("Введите 'commands' для списка доступных команд.\n");

    if (argc > 1) // есть аргументы командной строки
    {
        if (strcmp(argv[1], "book") == 0 && argc == 4) // пример: ./lock book 1 4
        { book_seat(atoi(argv[2]), atoi(argv[3])); } // atoi - строка в число
        else if (strcmp(argv[1], "cancel") == 0 && argc == 4) // пример: ./lock cancel 1 4
        {
            printf("Для отмены используйте интерактивный режим\n");
            close(file_descriptor); return EXIT_FAILURE;
        }
        else // некорректные аргументы
        {
            printf("Применение:\n");
            printf("%s book <ряд> <место>\n", argv[0]);
            close(file_descriptor); return EXIT_FAILURE;
        }
    }

    char cmd_buf[50]; // буфер для команды
	while (1) // бесконечный цикл
	{
		printf("\noperator> "); // приглашение ввода
		fflush(stdout); // принудительный сброс буфера

		if (scanf("%s", cmd_buf) != 1) break; // читаем команду, ошибка - выход

		if (strcmp(cmd_buf, "quit") == 0 || strcmp(cmd_buf, "exit") == 0)
		{
			if (active_reservations > 0) { release_all_locks(); } // снимаем блокировки
			reset_all_seats(); // восстанавливаем файл
			printf("Выход из системы.\n");
			break; // выход из цикла
		}
		else if (strcmp(cmd_buf, "commands") == 0) { show_commands(); }
		else if (strcmp(cmd_buf, "info") == 0) { show_info(); }
		else if (strcmp(cmd_buf, "cancel_all") == 0) { cancel_all_command(); }
		else if (strcmp(cmd_buf, "cancel") == 0)
		{
			int r, p;
			if (scanf("%d %d", &r, &p) != 2) // читаем ряд и место
			{
				printf("Применение: cancel <ряд> <место>\n");
				int ch; while ((ch = getchar()) != '\n' && ch != EOF); // очистка буфера
				continue;
			}
			cancel_seat(r, p);
		}
		else if (strcmp(cmd_buf, "book") == 0)
		{
			int r, p;
			if (scanf("%d %d", &r, &p) != 2) // читаем ряд и место
			{
				printf("Применение: book <ряд> <место>\n");
				int ch; while ((ch = getchar()) != '\n' && ch != EOF); // очистка буфера
				continue;
			}
			book_seat(r, p);
		}
		else if (strcmp(cmd_buf, "verify") == 0)
		{
			int r, p;
			if (scanf("%d %d", &r, &p) != 2) // читаем ряд и место
			{
				printf("Применение: verify <ряд> <место>\n");
				int ch; while ((ch = getchar()) != '\n' && ch != EOF); // очистка буфера
				continue;
			}
			verify_seat(r, p);
		}
		else // неизвестная команда
		{
			printf("Неизвестная команда. Введите 'commands' для справки.\n");
			// очистка входного буфера
			int ch; while ((ch = getchar()) != '\n' && ch != EOF);
		}
	}

    close(file_descriptor); // закрываем файл
    return EXIT_SUCCESS; // успешное завершение
}
