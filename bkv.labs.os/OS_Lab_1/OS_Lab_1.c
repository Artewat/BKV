#include <stdio.h> // printf, perror, fflush - функции стандартного ввода/вывода
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE - макросы кодов завершения
#include <stdint.h> // uint8_t, uint16_t, uint32_t - типы данных фиксированной длины
#include <string.h> // работа со строками
#include <fcntl.h> // open - открытие файлов
#include <unistd.h> // read, lseek, close - операции с файлами

/* Описание одного раздела таблицы MBR (16 байт) */
typedef struct
{
    uint8_t boot_indicator;   /* 0x80 = активный, 0x00 = неактивный */
    uint8_t chs_first[3];     /* CHS-адрес первого сектора (устаревшее) */
    uint8_t partition_type;   /* идентификатор типа раздела */
    uint8_t chs_last[3];      /* CHS-адрес последнего сектора (устаревшее) */
    uint32_t lba_start;       /* LBA-адрес первого сектора */
    uint32_t lba_count;       /* общее количество секторов */
} __attribute__((packed)) PartRecord;

/* Главная загрузочная запись — 512 байт */
typedef struct
{
    uint8_t boot_code[446];           /* область загрузочного кода */
    PartRecord table[4];              /* четыре записи о разделах */
    uint16_t magic_number;            /* контрольная сигнатура 0xAA55 */
} __attribute__((packed)) MasterBootRecord;


/* Преобразование кода раздела в текстовое описание */
const char* decode_partition_type(uint8_t type)
{
    switch(type)
    {
        case 0x00: return "Не используется";
        case 0x01: return "FAT12";
        case 0x04: return "FAT16 (до 32 МБ)";
        case 0x05: return "Расширенный";
        case 0x06: return "FAT16 (от 32 МБ)";
        case 0x07: return "NTFS / HPFS";
        case 0x0b: return "FAT32";
        case 0x0c: return "FAT32 (LBA)";
        case 0x0e: return "FAT16 (LBA)";
        case 0x0f: return "Расширенный (LBA)";
        case 0x82: return "Linux swap";
        case 0x83: return "Linux ext2/3/4";
        case 0x8e: return "Linux LVM";
        case 0xfd: return "Linux RAID";
        default: return "Неизвестный тип";
    }
}

/* Проверка сигнатуры загрузочного сектора */
int verify_mbr_signature(const MasterBootRecord* mbr)
{ return (mbr->magic_number == 0xAA55) ? 1 : 0; }

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Использование: %s <образ_диска>\n", argv[0]);
        fprintf(stderr, "Пример: %s disk_image.img\n", argv[0]);
        return EXIT_FAILURE;
    }

    int disk_fd = open(argv[1], O_RDONLY);
    if (disk_fd < 0)
    {
        perror("Не удалось открыть файл образа");
        return EXIT_FAILURE;
    }

    printf("\n=== Просмотр таблицы разделов: %s ===\n", argv[1]);
    printf("Размер сектора: 512 байт\n");
    fflush(stdout);

    MasterBootRecord mbr;

    /* Переходим к началу файла */
    if (lseek(disk_fd, 0, SEEK_SET) == -1)
    {
        perror("Сбой при позиционировании");
        close(disk_fd);
        return EXIT_FAILURE;
    }

    /* Считываем первый сектор */
    ssize_t bytes_read = read(disk_fd, &mbr, sizeof(MasterBootRecord));
    if (bytes_read != sizeof(MasterBootRecord))
    {
        perror("Не удалось прочитать MBR");
        close(disk_fd);
        return EXIT_FAILURE;
    }

    /* Проверяем контрольную сигнатуру */
    if (!verify_mbr_signature(&mbr))
    {
        fprintf(stderr, "Сигнатура 0xAA55 отсутствует — файл не является корректным MBR-образом\n");
        fflush(stdout);
        close(disk_fd);
        return EXIT_FAILURE;
    }

    printf("Сигнатура MBR: 0xAA55 — корректна\n");
    printf("Количество записей: 4\n\n");
    fflush(stdout);

    int found_count = 0;

    for (int idx = 0; idx < 4; idx++)
    {
        PartRecord *rec = &mbr.table[idx];

        if (rec->partition_type != 0x00)
        {
            found_count++;
            const char* type_name = decode_partition_type(rec->partition_type);
            const char* is_bootable = (rec->boot_indicator == 0x80) ? "да" : "нет";

            printf("─── Запись %d ───\n", idx + 1);

            if (rec->partition_type == 0x05 || rec->partition_type == 0x0f)
            {
                printf("Тип: расширенный раздел (содержит логические диски)\n");
            }

            printf("Загрузочный: %s\n", is_bootable);
            printf("Тип ФС: %s (0x%02X)\n", type_name, rec->partition_type);
            printf("Первый сектор (LBA): %u\n", rec->lba_start);
            printf("Секторов: %u\n", rec->lba_count);

            unsigned long long total_bytes = (unsigned long long)rec->lba_count * 512ULL;
            double total_mb = (double)total_bytes / (1024.0 * 1024.0);
            printf("Объём: %.2f МБ\n\n", total_mb);
        }
    }

    if (found_count == 0)
    {
        printf("Разделы не обнаружены\n");
    }
    else
    {
        printf("Найдено разделов: %d\n", found_count);
    }
    fflush(stdout);

    if (close(disk_fd) < 0)
    {
        perror("Ошибка при закрытии файла");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
