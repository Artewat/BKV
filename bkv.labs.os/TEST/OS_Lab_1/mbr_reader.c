#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

/* MBR занимает 512 байт, сигнатура — последние 2 байта */
#define MBR_SIZE 512
#define MBR_SIG_OFFSET 510
#define MBR_SIG 0xAA55

/* Таблица разделов начинается со смещения 446 */
#define PARTITION_TABLE_OFFSET 446
#define PARTITION_ENTRY_SIZE 16
#define NUM_PARTITIONS 4

/* Описание записи раздела в таблице MBR */
#pragma pack(push, 1)
typedef struct {
    uint8_t  boot_indicator;   /* 0x80 — загрузочный, 0x00 — нет */
    uint8_t  chs_start[3];     /* CHS-адрес начала (legacy) */
    uint8_t  partition_type;   /* Тип файловой системы */
    uint8_t  chs_end[3];       /* CHS-адрес конца (legacy) */
    uint32_t lba_start;        /* LBA начала раздела */
    uint32_t lba_size;         /* Размер раздела в секторах LBA */
} PartitionEntry;
#pragma pack(pop)

/* Расширение записей о типах разделов в понятные названия ФС */
const char *get_filesystem_name(uint8_t type) {
    switch (type) {
        case 0x00: return "Пустой (не используется)";
        case 0x01: return "FAT12";
        case 0x04: return "FAT16 (< 32 МБ)";
        case 0x05: return "Расширенный раздел (Extended)";
        case 0x06: return "FAT16 (>= 32 МБ)";
        case 0x07: return "NTFS / exFAT / HPFS";
        case 0x0B: return "FAT32";
        case 0x0C: return "FAT32 (LBA)";
        case 0x0E: return "FAT16 (LBA)";
        case 0x0F: return "Расширенный раздел (LBA)";
        case 0x17: return "IFS / Hidden NTFS";
        case 0x1C: return "Hidden W95 FAT32 (LBA)";
        case 0x1E: return "Hidden W95 FAT16 (LBA)";
        case 0x27: return "Windows Recovery Environment";
        case 0x42: return "MBR динамического диска";
        case 0x82: return "Linux Swap";
        case 0x83: return "Linux (ext2/3/4, ReiserFS и др.)";
        case 0x85: return "Linux Extended";
        case 0x8E: return "Linux LVM";
        case 0xA5: return "FreeBSD";
        case 0xA8: return "Mac OS X UFS";
        case 0xAB: return "Mac OS X boot";
        case 0xAF: return "Mac OS X HFS+";
        case 0xEF: return "EFI System Partition";
        case 0xFD: return "Linux RAID autodetect";
        default:   return "Неизвестный тип";
    }
}

int main(int argc, char *argv[]) {
    int fd;
    uint8_t mbr[MBR_SIZE];
    ssize_t bytes_read;

    /* Проверка аргументов командной строки */
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <файл_с_копией_MBR>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Открытие файла (только для чтения — диск не затрагивается) */
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    /* Чтение загрузочного сектора */
    bytes_read = read(fd, mbr, MBR_SIZE);
    if (bytes_read != MBR_SIZE) {
        fprintf(stderr, "Ошибка чтения: ожидалось %d байт, прочитано %zd\n",
                MBR_SIZE, bytes_read);
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    /* Проверка сигнатуры MBR */
    uint16_t signature = *(uint16_t *)(mbr + MBR_SIG_OFFSET);
    if (signature != MBR_SIG) {
        fprintf(stderr,
                "Ошибка: сигнатура загрузочного сектора не найдена "
                "(ожидается 0xAA55, получено 0x%04X)\n",
                signature);
        return EXIT_FAILURE;
    }

    printf("=== Загрузочная запись (MBR) ===\n");
    printf("Сигнатура сектора: 0x%04X (OK)\n\n", signature);

    /* Чтение и вывод информации о разделах */
    PartitionEntry *partitions = (PartitionEntry *)(mbr + PARTITION_TABLE_OFFSET);

    printf("№ | Загрузочный | Тип ФС                | Начало (LBA) | Размер (LBA)\n");
    printf("--|-------------|-----------------------|--------------|-------------\n");

    for (int i = 0; i < NUM_PARTITIONS; i++) {
        PartitionEntry *p = &partitions[i];

        /* Пропускаем пустые записи */
        if (p->partition_type == 0x00) {
            printf("%d |      -      | (пусто)               |      -       |      -\n",
                   i + 1);
            continue;
        }

        /* Флаг загрузочности */
        const char *boot_flag = (p->boot_indicator == 0x80) ? "Да (*)" : "Нет";

        /* Информация о разделе */
        printf("%d | %11s | %-21s | %12u | %12u\n",
               i + 1,
               boot_flag,
               get_filesystem_name(p->partition_type),
               p->lba_start,
               p->lba_size);
    }

    printf("\n(*) — раздел помечен как загрузочный.\n");

    fflush(stdout);

    return EXIT_SUCCESS;
}
