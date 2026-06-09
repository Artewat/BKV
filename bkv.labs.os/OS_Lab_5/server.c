#include <stdio.h>   // вывод сообщений в консоль
#include <stdlib.h>  // завершение программы, преобразование строк
#include <string.h>  // работа с памятью и строками
#include <unistd.h>  // POSIX-вызовы: закрытие, чтение, запись
#include <sys/socket.h>  // создание и работа с сокетами
#include <netinet/in.h>  // интернет-адреса
#include <arpa/inet.h>   // конвертация IP-адресов в строки
#include <sys/select.h>  // мониторинг нескольких файловых дескрипторов

#define MAX_CONNECTIONS 10  // лимит одновременных подключений
#define MAX_BUFFER 1024     // размер приемного буфера

typedef struct
{
    int socket_fd;          // файловый дескриптор (-1 если слот пуст)
    char addr_str[16];      // IP-адрес в текстовом виде
    char node_id[16];       // автоматический идентифатор узла (например, N5)
    char display_name[32];  // зарегистрированное имя пользователя
    int connected;          // статус подключения (1 - активен, 0 - неактивен)
} Participant;

Participant participants[MAX_CONNECTIONS];  // таблица активных участников

// Отправка сообщения указанному дескриптору
void dispatch_message(int target_fd, const char *text) { send(target_fd, text, strlen(text), 0); }

// Начальная инициализация таблицы участников
void reset_participants()
{
    for (int idx = 0; idx < MAX_CONNECTIONS; idx++)
    {
        participants[idx].socket_fd = -1; participants[idx].connected = 0;
        memset(participants[idx].addr_str, 0, sizeof(participants[idx].addr_str));
        memset(participants[idx].node_id, 0, sizeof(participants[idx].node_id));
        memset(participants[idx].display_name, 0, sizeof(participants[idx].display_name));
    }
}

// Генерация идентификатора узла на основе последней октеты IP
void generate_node_id(const char *ip_addr, char *id_buffer)
{
    strcpy(id_buffer, "UNK");  // значение по умолчанию
    if (strcmp(ip_addr, "127.0.0.1") == 0) { strcpy(id_buffer, "L0"); return; }

    const char *dot_pos = strrchr(ip_addr, '.');
    if (dot_pos)
    {
        int last_part = atoi(dot_pos + 1);
        snprintf(id_buffer, 16, "N%d", last_part % 100);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) { fprintf(stderr, "Применение: %s <номер_порта>\n", argv[0]); exit(1); }
    int listen_port = atoi(argv[1]);

    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    char input_buffer[MAX_BUFFER];
    fd_set active_fds;
    reset_participants();

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) { perror("Не удалось создать сокет"); exit(1); }
    int reuse = 1; setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(listen_port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { perror("Не удалось привязать адрес"); exit(2); }
    if (listen(server_socket, MAX_CONNECTIONS) < 0) { perror("Не удалось начать прослушивание"); exit(3); }
    printf("[Сервер] Узел активен на порту %d. Ожидание подключений...\n", listen_port);

    while (1)
    {
        FD_ZERO(&active_fds);
        FD_SET(server_socket, &active_fds);
        int top_fd = server_socket;

        for (int idx = 0; idx < MAX_CONNECTIONS; idx++)
        { if (participants[idx].socket_fd > 0) { FD_SET(participants[idx].socket_fd, &active_fds); if (participants[idx].socket_fd > top_fd) top_fd = participants[idx].socket_fd; } }
        if (select(top_fd + 1, &active_fds, NULL, NULL, NULL) < 0) { perror("Сбой select"); continue; }

        if (FD_ISSET(server_socket, &active_fds))
        {
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
            if (client_socket < 0) continue; char *remote_ip = inet_ntoa(client_addr.sin_addr);

            // Фильтрация по подсети (допускаем 10.40.14.0/24 и loopback)
            if (strncmp(remote_ip, "10.40.14.", 9) != 0 && strcmp(remote_ip, "127.0.0.1") != 0)
            { printf("[Отказ] Блокировка внешней подсети: %s\n", remote_ip); close(client_socket); continue; }

            int slot_found = 0;
            for (int idx = 0; idx < MAX_CONNECTIONS; idx++)
            {
                if (participants[idx].socket_fd == -1)
                {
                    participants[idx].socket_fd = client_socket; strcpy(participants[idx].addr_str, remote_ip);
                    generate_node_id(remote_ip, participants[idx].node_id);
                    participants[idx].connected = 0; slot_found = 1;

                    dispatch_message(client_socket, "Вход выполнен. Представьтесь (введите имя):\n");
                    printf("[Подключение] %s (Узел: %s)\n", remote_ip, participants[idx].node_id); break;
                }
            }
            if (!slot_found) { dispatch_message(client_socket, "Превышен лимит участников. Повторите попытку позже.\n"); close(client_socket); }
        }

        for (int idx = 0; idx < MAX_CONNECTIONS; idx++)
        {
            int curr_fd = participants[idx].socket_fd;
            if (curr_fd > 0 && FD_ISSET(curr_fd, &active_fds))
            {
                int bytes_in = recv(curr_fd, input_buffer, MAX_BUFFER - 1, 0);

                if (bytes_in <= 0)
                {
                    if (participants[idx].connected)
                    {
                        char announce[MAX_BUFFER];
                        snprintf(announce, sizeof(announce), ">>> Участник %s отключился от сети!\n", participants[idx].display_name);
                        for (int j = 0; j < MAX_CONNECTIONS; j++) { if (participants[j].connected && participants[j].socket_fd != curr_fd) dispatch_message(participants[j].socket_fd, announce); }
                    }

                    printf("[Отключение] %s (%s)\n", participants[idx].display_name[0] ? participants[idx].display_name : "Аноним", participants[idx].addr_str);
                    close(curr_fd); participants[idx].socket_fd = -1; participants[idx].connected = 0;
                    memset(participants[idx].display_name, 0, sizeof(participants[idx].display_name));
                }
                else
                {
                    input_buffer[bytes_in] = '\0';
                    input_buffer[strcspn(input_buffer, "\r\n")] = 0;
                    if (strlen(input_buffer) == 0) continue;

                    if (participants[idx].connected == 0)
                    {
                        strncpy(participants[idx].display_name, input_buffer, 31); participants[idx].connected = 1;
                        printf("[Регистрация] %s [%s | %s]\n", participants[idx].display_name, participants[idx].addr_str, participants[idx].node_id);

                        char notification[MAX_BUFFER];
                        snprintf(notification, sizeof(notification), ">>> Участник %s вошел в сеть!\n", participants[idx].display_name);
                        for (int j = 0; j < MAX_CONNECTIONS; j++) { if (participants[j].connected && participants[j].socket_fd != curr_fd) dispatch_message(participants[j].socket_fd, notification); }

                        dispatch_message(curr_fd, "Авторизация успешна. Команды: /list, /msg <ник|IP|узел> <текст>, /all <текст>, /quit\n");
                        continue;
                    }

                    if (strncmp(input_buffer, "/list", 5) == 0)
                    {
                        dispatch_message(curr_fd, "Список участников сети\n");
                        for (int j = 0; j < MAX_CONNECTIONS; j++)
                        {
                            if (participants[j].connected)
                            {
                                char entry[128];
                                snprintf(entry, sizeof(entry), "- Имя: %s | Узел: %s | Адрес: %s\n", participants[j].display_name, participants[j].node_id, participants[j].addr_str);
                                dispatch_message(curr_fd, entry);
                            }
                        }
                    }
                    else if (strncmp(input_buffer, "/msg", 4) == 0)
                    {
                        if (strlen(input_buffer) <= 5 || input_buffer[4] != ' ')
                        { dispatch_message(curr_fd, "Применение: /msg <ник|IP|узел> <текст>\n"); }
                        else
                        {
                            char *recipient = strtok(input_buffer + 5, " ");
                            char *payload = strtok(NULL, "");

                            if (recipient && payload)
                            {
                                int matched = 0;
                                for (int j = 0; j < MAX_CONNECTIONS; j++)
                                {
                                    if (participants[j].connected && (
                                        strcmp(participants[j].display_name, recipient) == 0 ||
                                        strcmp(participants[j].node_id, recipient) == 0 ||
                                        strcmp(participants[j].addr_str, recipient) == 0))
                                    {
                                        char private_msg[MAX_BUFFER];
                                        snprintf(private_msg, sizeof(private_msg), "[Личное от %s]: %s\n", participants[idx].display_name, payload);
                                        dispatch_message(participants[j].socket_fd, private_msg); matched = 1; break;
                                    }
                                }
                                if (!matched) { dispatch_message(curr_fd, "Ошибка: Адресат не найден.\n"); }
                                else { dispatch_message(curr_fd, ">>> Доставка подтверждена.\n"); }
                            }
                            else { dispatch_message(curr_fd, "Применение: /msg <ник|IP|узел> <текст>\n"); }
                        }
                    }
                    else if (strncmp(input_buffer, "/all", 4) == 0)
                    {
                        if (strlen(input_buffer) <= 5 || input_buffer[4] != ' ')
                        { dispatch_message(curr_fd, "Применение: /all <текст>\n"); }
                        else
                        {
                            char *broadcast_text = input_buffer + 5; char broadcast_msg[MAX_BUFFER];
                            snprintf(broadcast_msg, sizeof(broadcast_msg), "[Эфир от %s]: %s\n", participants[idx].display_name, broadcast_text);
                            for (int j = 0; j < MAX_CONNECTIONS; j++) { if (participants[j].connected && participants[j].socket_fd != curr_fd) dispatch_message(participants[j].socket_fd, broadcast_msg); }
                            dispatch_message(curr_fd, ">>> Широковещательная рассылка выполнена.\n");
                        }
                    }
                    else if (strncmp(input_buffer, "/quit", 5) == 0)
                    {
                        char leave_msg[MAX_BUFFER];
                        snprintf(leave_msg, sizeof(leave_msg), ">>> Участник %s покинул сеть!\n", participants[idx].display_name);
                        for (int j = 0; j < MAX_CONNECTIONS; j++) { if (participants[j].connected && participants[j].socket_fd != curr_fd) dispatch_message(participants[j].socket_fd, leave_msg); }

                        printf("[Выход] %s (%s)\n", participants[idx].display_name, participants[idx].addr_str);
                        close(curr_fd); participants[idx].socket_fd = -1; participants[idx].connected = 0;
                        memset(participants[idx].display_name, 0, sizeof(participants[idx].display_name));
                    }
                    else { dispatch_message(curr_fd, "Команда не распознана. Список: /list, /msg, /all, /quit\n"); }
                }
            }
        }
    }
    return 0;
}
