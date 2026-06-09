#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int depth_level;
pid_t children_pids[256];
int active_children = 0;

// обработчик SIGCHLD - срабатывает при завершении дочернего процесса
void sigchld_handler(int sig)
{
    int errno_backup = errno;
    pid_t child_pid;
    int status;

    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("[PID %d] Замечено завершение потомка PID=%d\n", getpid(), child_pid);

        for (int idx = 0; idx < active_children; idx++)
        {
            if (children_pids[idx] == child_pid)
            {
                for (int k = idx; k < active_children - 1; k++)
                {
                    children_pids[k] = children_pids[k + 1];
                }
                active_children--;
                printf("[PID %d] Потомок PID=%d исключён из списка\n", getpid(), child_pid);
                break;
            }
        }
    }
    errno = errno_backup;
}

// обработчик SIGALRM - отображает состояние процесса и его потомков
void sigalrm_handler(int sig)
{
    printf("[PID %d] *** ПРИНЯТ SIGALRM ***\n", getpid());
    printf("[PID %d] Процесс активен! (глубина %d, предок %d)\n", getpid(), depth_level, getppid());

    if (active_children > 0)
    {
        printf("[PID %d] Список потомков:", getpid());
        for (int idx = 0; idx < active_children; idx++)
        {
            if (kill(children_pids[idx], 0) == 0)
            {
                printf(" %d", children_pids[idx]);
            }
        }
        printf("\n");

        for (int idx = 0; idx < active_children; idx++)
        {
            if (kill(children_pids[idx], 0) == 0)
            {
                printf("[PID %d] SIGALRM направлен потомку %d\n", getpid(), children_pids[idx]);
            }
        }
        fflush(stdout);

        for (int idx = 0; idx < active_children; idx++)
        {
            if (kill(children_pids[idx], 0) == 0)
            {
                kill(children_pids[idx], SIGALRM);
                usleep(100000);
            }
        }
    }
    else
    {
        printf("[PID %d] Дочерние процессы отсутствуют\n", getpid());
    }
}

// обработчик SIGUSR1 - каскадное заверение себя и всех потомков
void sigusr1_handler(int sig)
{
    printf("[PID %d] *** ПРИНЯТ SIGUSR1 (сигнал остановки) ***\n", getpid());
    printf("[PID %d] Завершение работы: глубина %d, предок %d\n", getpid(), depth_level, getppid());

    if (active_children > 0)
    {
        int snapshot_count = active_children;
        pid_t snapshot_pids[256];
        for (int idx = 0; idx < snapshot_count; idx++)
        {
            snapshot_pids[idx] = children_pids[idx];
        }

        printf("[PID %d] Остановка дочерних процессов:", getpid());
        for (int idx = 0; idx < snapshot_count; idx++)
        {
            if (kill(snapshot_pids[idx], 0) == 0)
            {
                printf(" %d", snapshot_pids[idx]);
            }
        }
        printf("\n");
        fflush(stdout);

        for (int idx = 0; idx < snapshot_count; idx++)
        {
            if (kill(snapshot_pids[idx], 0) == 0)
            {
                kill(snapshot_pids[idx], SIGUSR1);
                usleep(100000);
            }
        }

        while (active_children > 0)
        {
            usleep(10000);
        }
    }

    printf("[PID %d] Процесс остановлен\n", getpid());
    fflush(stdout);
    _exit(0);
}

// обработчик SIGUSR2 - порождение нового дочернего процесса
void sigusr2_handler(int sig)
{
    if (depth_level > 0)
    {
        printf("[PID %d] *** ПРИНЯТ SIGUSR2 (запрос на создание) ***\n", getpid());
        fflush(stdout);

        pid_t new_pid = fork();
        if (new_pid == -1)
        {
            perror("Сбой fork()");
            return;
        }

        if (new_pid == 0)
        {
            char lvl[16];
            sprintf(lvl, "%d", depth_level - 1);

            execlp("./child", "child", lvl, NULL);
            perror("Сбой execlp()");
            _exit(EXIT_FAILURE);
        }
        else
        {
            children_pids[active_children++] = new_pid;
            printf("[PID %d] Создан потомок PID=%d, глубина=%d\n", getpid(), new_pid, depth_level - 1);
        }
    }
    else
    {
        printf("[PID %d] *** ПРИНЯТ SIGUSR2 (отказ: нулевая глубина) ***\n", getpid());
        fflush(stdout);
    }
}

// начальное порождение дочерних процессов при запуске
void spawn_initial_children(void)
{
    for (int idx = 0; idx < depth_level; idx++)
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
            char lvl[16];
            sprintf(lvl, "%d", depth_level - 1);

            execlp("./child", "child", lvl, NULL);
            perror("Сбой execlp()");
            _exit(EXIT_FAILURE);
        }
        else
        {
            children_pids[active_children++] = new_pid;
            printf("[PID %d] Инициализирован потомок PID=%d, глубина=%d\n", getpid(), new_pid, depth_level - 1);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Ошибка: требуется один параметр - глубина вложенности\n");
        return EXIT_FAILURE;
    }

    depth_level = atoi(argv[1]);

    signal(SIGCHLD, sigchld_handler);
    signal(SIGALRM, sigalrm_handler);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    printf("[PID %d] ЗАПУСК: глубина %d, предок %d\n", getpid(), depth_level, getppid());
    
    if (depth_level > 0)
    {
        spawn_initial_children();
    }
    
    for (;;)
    {
        sleep(1);
    }

    return EXIT_SUCCESS;
}
