#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>

int max_depth;
pid_t child_pids[256];
int living_children = 0;
int spawned_count = 0;
int expected_total = 0;

// проверка существования процесса
int process_exists(pid_t pid)
{
    return (kill(pid, 0) == 0) ? 1 : 0;
}

// расчёт общего количества процессов (рекурсивно)
int calc_total_processes(int lvl)
{
    if (lvl <= 0) return 1;
    int total = 1;
    for (int idx = 0; idx < lvl; idx++)
    {
        total += calc_total_processes(lvl - 1);
    }
    return total;
}

// обработчик SIGCHLD - реакция на завершение потомка
void sigchld_handler(int sig)
{
    int errno_save = errno;
    pid_t terminated_pid;
    int status;

    while ((terminated_pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("[РОДИТЕЛЬ] Зафиксировано завершение PID=%d\n", terminated_pid);

        for (int idx = 0; idx < living_children; idx++)
        {
            if (child_pids[idx] == terminated_pid)
            {
                for (int k = idx; k < living_children - 1; k++)
                {
                    child_pids[k] = child_pids[k + 1];
                }
                living_children--;
                printf("[РОДИТЕЛЬ] Потомок PID=%d удалён из реестра\n", terminated_pid);
                break;
            }
        }
    }
    errno = errno_save;
}

// рекурсивный обход /proc для отображения дерева процессов
void dump_proc_tree(pid_t root_pid, int indent_level)
{
    for (int sp = 0; sp < indent_level; sp++)
    {
        printf("    ");
    }

    if (indent_level == 0)
    {
        printf("ГЛАВНЫЙ: PID=%d\n", root_pid);
    }
    else
    {
        printf("└─ ВЕТВЬ: PID=%d\n", root_pid);
    }

    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL) return;

    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL)
    {
        int is_numeric = 1;
        for (int ch = 0; entry->d_name[ch] != '\0'; ch++)
        {
            if (!isdigit(entry->d_name[ch]))
            {
                is_numeric = 0;
                break;
            }
        }
        
        if (is_numeric)
        {
            char stat_file[300];
            sprintf(stat_file, "/proc/%s/stat", entry->d_name);

            FILE *fp = fopen(stat_file, "r");
            if (fp != NULL)
            {
                int pid;
                char name[300];
                char state;
                int parent_pid;

                if (fscanf(fp, "%d (%[^)]) %c %d", &pid, name, &state, &parent_pid) == 4)
                {
                    if (parent_pid == root_pid)
                    {
                        dump_proc_tree(pid, indent_level + 1);
                    }
                }
                fclose(fp);
            }
        }
    }
    closedir(proc_dir);
}

// обработчик SIGALRM для родителя - статус процесса и потомков
void parent_sigalrm_handler(int sig)
{
    printf("[РОДИТЕЛЬ] *** ПОЛУЧЕН SIGALRM ***\n");
    printf("[РОДИТЕЛЬ] Активен! (PID=%d, глубина=%d)\n", getpid(), max_depth);

    if (living_children > 0)
    {
        printf("[РОДИТЕЛЬ] Дочерние процессы:");
        for (int idx = 0; idx < living_children; idx++)
        {
            if (process_exists(child_pids[idx]))
            {
                printf(" %d", child_pids[idx]);
            }
        }
        printf("\n");

        for (int idx = 0; idx < living_children; idx++)
        {
            if (process_exists(child_pids[idx]))
            {
                printf("[РОДИТЕЛЬ] SIGALRM передан потомку %d\n", child_pids[idx]);
            }
        }
        fflush(stdout);

        for (int idx = 0; idx < living_children; idx++)
        {
            if (process_exists(child_pids[idx]))
            {
                kill(child_pids[idx], SIGALRM);
                usleep(100000);
            }
        }
    }
    else
    {
        printf("[РОДИТЕЛЬ] Дочерних процессов нет\n");
    }
}

// обработчик SIGUSR1 для родителя - остановка всех процессов
void parent_sigusr1_handler(int sig)
{
    printf("[РОДИТЕЛЬ] *** ПОЛУЧЕН SIGUSR1 (сигнал остановки) ***\n");
    printf("[РОДИТЕЛЬ] Завершение: PID=%d, глубина=%d\n", getpid(), max_depth);

    if (living_children > 0)
    {
        int snapshot_cnt = living_children;
        pid_t snapshot[256];
        for (int idx = 0; idx < snapshot_cnt; idx++)
        {
            snapshot[idx] = child_pids[idx];
        }

        printf("[РОДИТЕЛЬ] Останавливаю потомков:");
        for (int idx = 0; idx < snapshot_cnt; idx++)
        {
            if (process_exists(snapshot[idx]))
            {
                printf(" %d", snapshot[idx]);
            }
        }
        printf("\n");
        fflush(stdout);

        for (int idx = 0; idx < snapshot_cnt; idx++)
        {
            if (process_exists(snapshot[idx]))
            {
                kill(snapshot[idx], SIGUSR1);
                usleep(100000);
            }
        }

        while (living_children > 0)
        {
            usleep(10000);
        }
    }

    printf("[РОДИТЕЛЬ] Работа остановлена\n");
    fflush(stdout);
    exit(0);
}

// обработчик SIGUSR2 для родителя - создание нового потомка
void parent_sigusr2_handler(int sig)
{
    printf("[РОДИТЕЛЬ] *** ПОЛУЧЕН SIGUSR2 (создание потомка) ***\n");
    fflush(stdout);

    pid_t new_pid = fork();
    if (new_pid == -1)
    {
        perror("Сбой fork()");
        return;
    }

    if (new_pid == 0)
    {
        char depth_arg[16];
        sprintf(depth_arg, "%d", max_depth - 1);

        execlp("./child", "child", depth_arg, NULL);
        perror("Сбой execlp()");
        exit(EXIT_FAILURE);
    }
    else
    {
        child_pids[living_children++] = new_pid;
        printf("[РОДИТЕЛЬ] Создан потомок PID=%d, глубина=%d\n", new_pid, max_depth - 1);
    }
}

// вывод справки по командам
void show_help(void)
{
    printf("\nСписок доступных команд:\n");
    printf("a <PID> - отправить SIGALRM (статус процесса и потомков)\n");
    printf("k <PID> - отправить SIGUSR1 (остановка процесса и ветки)\n");
    printf("n <PID> - отправить SIGUSR2 (создать нового потомка)\n");
    printf("p - отобразить полное дерево процессов\n");
    printf("x - завершить все процессы и выйти\n");
    printf("help - показать данную справку\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Ошибка: укажите количество уровней потомков\n");
        printf("Пример: %s <число_уровней>\n", argv[0]);
        return EXIT_FAILURE;
    }

    max_depth = atoi(argv[1]);
    if (max_depth <= 0 || max_depth > 127)
    {
        printf("Ошибка: число уровней должно быть от 1 до 127\n");
        return EXIT_FAILURE;
    }

    expected_total = calc_total_processes(max_depth);
    signal(SIGCHLD, sigchld_handler);

    signal(SIGALRM, parent_sigalrm_handler);
    signal(SIGUSR1, parent_sigusr1_handler);
    signal(SIGUSR2, parent_sigusr2_handler);

    printf("\nРасчётное количество процессов: %d\n", expected_total);
    printf("РОДИТЕЛЬ: PID=%d запущен, строим дерево с %d уровнями\n", getpid(), max_depth);

    for (int idx = 0; idx < max_depth; idx++)
    {
        sleep(1);

        pid_t new_pid = fork();
        if (new_pid == -1)
        {
            perror("Сбой fork()");
            continue;
        }

        if (new_pid == 0)
        {
            char depth_arg[16];
            sprintf(depth_arg, "%d", max_depth - 1);

            execlp("./child", "child", depth_arg, NULL);
            perror("Сбой execlp()");
            exit(EXIT_FAILURE);
        }
        else
        {
            child_pids[living_children++] = new_pid;
            spawned_count++;
            printf("РОДИТЕЛЬ: создан потомок PID=%d, глубина=%d\n", new_pid, max_depth - 1);
        }
    }

    sleep(max_depth * 3);
    printf("\nВСЕ ПРОЦЕССЫ СОЗДАНЫ. Ожидаемое количество: %d\n", expected_total);
    printf("Введите 'help' для просмотра команд.\n");

    char cmd_buf[16];
    int target_pid;

    while (1)
    {
        printf("\nкомандная строка> ");
        fflush(stdout);
        
        if (scanf("%s", cmd_buf) != 1) break;

        if (strcmp(cmd_buf, "help") == 0)
        {
            show_help();
        }
        else if (strcmp(cmd_buf, "p") == 0)
        {
            printf("\nДЕРЕВО ПРОЦЕССОВ\n");
            dump_proc_tree(getpid(), 0);
        }
        else if (strcmp(cmd_buf, "x") == 0)
        {
            printf("\nРОДИТЕЛЬ: остановка всех процессов...\n");

            int snapshot_cnt = living_children;
            pid_t snapshot[256];
            for (int idx = 0; idx < snapshot_cnt; idx++)
            {
                snapshot[idx] = child_pids[idx];
            }

            for (int idx = 0; idx < snapshot_cnt; idx++)
            {
                if (process_exists(snapshot[idx]))
                {
                    printf("  SIGUSR1 отправлен процессу %d\n", snapshot[idx]);
                }
            }
            fflush(stdout);

            for (int idx = 0; idx < snapshot_cnt; idx++)
            {
                if (process_exists(snapshot[idx]))
                {
                    kill(snapshot[idx], SIGUSR1);
                    usleep(100000);
                }
            }

            while (living_children > 0)
            {
                usleep(10000);
            }

            printf("РОДИТЕЛЬ: работа завершена\n");
            break;
        }
        else if (strcmp(cmd_buf, "a") == 0 || strcmp(cmd_buf, "k") == 0 || strcmp(cmd_buf, "n") == 0)
        {
            if (scanf("%d", &target_pid) != 1)
            {
                printf("Ошибка: укажите PID процесса\n");
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                continue;
            }

            if (!process_exists(target_pid))
            {
                printf("Ошибка: процесс с PID=%d не найден\n", target_pid);
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                continue;
            }

            int sig_num = -1;
            char *sig_label = "";

            if (strcmp(cmd_buf, "a") == 0)
            {
                sig_num = SIGALRM;
                sig_label = "SIGALRM";
            }
            else if (strcmp(cmd_buf, "k") == 0)
            {
                sig_num = SIGUSR1;
                sig_label = "SIGUSR1";
            }
            else if (strcmp(cmd_buf, "n") == 0)
            {
                sig_num = SIGUSR2;
                sig_label = "SIGUSR2";
            }

            printf("\nСигнал %s направлен процессу %d\n", sig_label, target_pid);
            fflush(stdout);

            if (kill(target_pid, sig_num) == -1)
            {
                perror("Сбой отправки сигнала");
            }
            else
            {
                sleep(1);
            }

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        else
        {
            printf("Неизвестная команда. Введите 'help' для справки.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }

    while (living_children > 0)
    {
        usleep(10000);
    }
    
    printf("РОДИТЕЛЬ: процесс остановлен\n");
    return EXIT_SUCCESS;
}
