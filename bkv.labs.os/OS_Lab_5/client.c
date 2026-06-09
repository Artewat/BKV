#include <stdio.h>   // вывод сообщений в консоль
#include <stdlib.h>  // завершение программы, преобразование строк
#include <string.h>  // работа с памятью и строками
#include <unistd.h>  // POSIX-вызовы: закрытие, чтение, запись
#include <sys/socket.h>  // создание и работа с сокетами
#include <netinet/in.h>  // интернет-адреса
#include <arpa/inet.h>   // конвертация IP-адресов
#include <sys/select.h>  // мониторинг нескольких файловых дескрипторов

#define MAX_BUFFER 1024  // размер приемного буфера

int main(int argc, char **argv)
{
    if (argc != 3)
    { fprintf(stderr, "Применение: %s <адрес_узла> <номер_порта>\n", argv[0]); exit(1); }
    int target_port = atoi(argv[2]);

    int conn_socket; struct sockaddr_in remote_addr; char recv_buffer[MAX_BUFFER];
    conn_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_socket < 0) { perror("Не удалось создать сокет"); exit(1); }

    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(target_port);

    if (inet_pton(AF_INET, argv[1], &remote_addr.sin_addr) <= 0)
    { perror("Некорректный IP-адрес"); exit(1); }
    if (connect(conn_socket, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) < 0)
    { perror("Не удалось подключиться"); exit(2); }

    fd_set monitored_fds;
    int highest_fd = (conn_socket > STDIN_FILENO) ? conn_socket : STDIN_FILENO;

    while (1)
    {
        FD_ZERO(&monitored_fds);
        FD_SET(STDIN_FILENO, &monitored_fds);
        FD_SET(conn_socket, &monitored_fds);
        if (select(highest_fd + 1, &monitored_fds, NULL, NULL, NULL) < 0)
        { perror("Сбой select"); break; }

        if (FD_ISSET(conn_socket, &monitored_fds))
        {
            int received = recv(conn_socket, recv_buffer, MAX_BUFFER - 1, 0);
            if (received <= 0) { printf("\n[Система] Связь с узлом потеряна.\n"); break; }
            recv_buffer[received] = '\0';
            printf("%s", recv_buffer);
            fflush(stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &monitored_fds))
        {
            if (fgets(recv_buffer, MAX_BUFFER, stdin) != NULL)
            {
                send(conn_socket, recv_buffer, strlen(recv_buffer), 0);
                if (strncmp(recv_buffer, "/quit", 5) == 0)
                { printf("[Система] Завершение сеанса...\n"); break; }
            }
        }
    }

    close(conn_socket);
    return 0;
}
