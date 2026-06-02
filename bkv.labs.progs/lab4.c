#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 100

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point pos;
    int dist;
} QueueNode;

int M, N;
int maze[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];
Point prev[MAX_SIZE][MAX_SIZE];
const int dx[4] = {-1, 0, 1, 0};
const int dy[4] = {0, 1, 0, -1};
const int walls[4] = {8, 4, 2, 1}; // Север, Восток, Юг, Запад

void read_maze(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[MAX_SIZE + 2];
    M = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') line[len - 1] = '\0';
        N = strlen(line);
        for (int j = 0; j < N; j++) {
            maze[M][j] = line[j] - '0'; // Преобразуем символ цифры в число
        }
        M++;
    }
    fclose(file);
}

bool is_valid(int x, int y) {
    return x >= 0 && x < M && y >= 0 && y < N;
}

bool has_wall(int x, int y, int dir) {
    return maze[x][y] & walls[dir];
}

void bfs() {
    QueueNode queue[MAX_SIZE * MAX_SIZE];
    int front = 0, rear = 0;
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            visited[i][j] = false;
            prev[i][j] = (Point){-1, -1};
        }
    }
    
    queue[rear++] = (QueueNode){{0, 0}, 0};
    visited[0][0] = true;
    
    while (front < rear) {
        QueueNode current = queue[front++];
        Point pos = current.pos;
        
        if (pos.x == M - 1 && pos.y == N - 1) {
            printf("Длина кратчайшего пути: %d\n", current.dist);
            return;
        }
        
        for (int dir = 0; dir < 4; dir++) {
            int nx = pos.x + dx[dir];
            int ny = pos.y + dy[dir];
            
            if (is_valid(nx, ny) && !visited[nx][ny] && !has_wall(pos.x, pos.y, dir)) {
                visited[nx][ny] = true;
                prev[nx][ny] = pos;
                queue[rear++] = (QueueNode){{nx, ny}, current.dist + 1};
            }
        }
    }
    
    printf("Путь не найден!\n");
}

void mark_path() {
    char path_map[MAX_SIZE][MAX_SIZE + 1];
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            path_map[i][j] = maze[i][j] + '0'; // Преобразуем число обратно в символ
        }
        path_map[i][N] = '\0';
    }
    
    Point current = {M - 1, N - 1};
    path_map[current.x][current.y] = '*';
    
    while (current.x != 0 || current.y != 0) {
        Point p = prev[current.x][current.y];
        if (p.x == -1 && p.y == -1) {
            printf("Нет пути для отметки\n");
            return;
        }
        path_map[p.x][p.y] = '*';
        current = p;
    }
    
    FILE* out = fopen("output.txt", "w");
    if (!out) {
        perror("Ошибка создания файла output.txt");
        return;
    }
    
    for (int i = 0; i < M; i++) {
        fprintf(out, "%s\n", path_map[i]);
    }
    fclose(out);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Использование: %s <файл_лабиринта>\n", argv[0]);
        return 1;
    }
    
    read_maze(argv[1]);
    bfs();
    mark_path();
    
    return 0;
}