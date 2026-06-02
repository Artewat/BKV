#define _GNU_SOURCE
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>

#define MAX_DEPTH 100
#define IS_DOT_DIR(name) ((name)[0] == '.' && ((name)[1] == '\0' || ((name)[1] == '.' && (name)[2] == '\0')))

static char path_buffer[PATH_MAX];
static long empty_files_count = 0;

void write_number(int fd, long num) {
    char buf[32];
    char *ptr = buf + sizeof(buf);
    *--ptr = '\0';
    if (num == 0) {
        *--ptr = '0';
    } else {
        while (num > 0) {
            *--ptr = '0' + (num % 10);
            num /= 10;
        }
    }
    write(fd, ptr, strlen(ptr));
}

void scan_directory(size_t path_len, int depth) {
    if (depth > MAX_DEPTH || path_len >= PATH_MAX - 1) return;

    DIR *dir = opendir(path_buffer);
    if (!dir) return;

    struct dirent *entry;
    struct stat st;

    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;
        if (IS_DOT_DIR(name)) continue;

        size_t name_len = strlen(name);
        size_t new_len = path_len + 1 + name_len;
        if (new_len >= PATH_MAX) continue;

        path_buffer[path_len] = '/';
        strcpy(path_buffer + path_len + 1, name);

        if (stat(path_buffer, &st) == -1) continue;

        if (S_ISREG(st.st_mode) && st.st_size == 0) {
            int fd = open(path_buffer, O_RDONLY);
            if (fd != -1) {
                write(STDOUT_FILENO, path_buffer, new_len);
                write(STDOUT_FILENO, "\n", 1);
                close(fd);
                empty_files_count++;
            }
        } else if (S_ISDIR(st.st_mode)) {
            scan_directory(new_len, depth + 1);
        }
    }

    closedir(dir);
}

int main() {
    char *home = getenv("HOME");
    if (!home) {
        const char *msg = "Ошибка: HOME не задана\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return 1;
    }

    size_t home_len = strlen(home);
    if (home_len >= PATH_MAX - 1) {
        const char *msg = "Ошибка: путь HOME слишком длинный\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return 1;
    }

    strcpy(path_buffer, home);

    if (chdir(home) == -1) {
        const char *msg = "Ошибка: не удалось выполнить chdir\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return 1;
    }

    scan_directory(home_len, 0);

    const char *summary = "\nВсего файлов с нулевым размером: ";
    write(STDOUT_FILENO, summary, strlen(summary));
    write_number(STDOUT_FILENO, empty_files_count);
    write(STDOUT_FILENO, "\n", 1);

    return 0;
}