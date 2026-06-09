#include <stdlib.h> // exit - коды возврата
#include <unistd.h> // usleep, getpid - утилиты
#include <sys/shm.h> // shmat - общая память
#include <time.h> // time - инициализация ГСЧ
#include "common.h" // общие определения

// поиск наиболее богатой клетки в окрестности (тор)
void locate_best_tile(SimShared *s, int cx, int cy, int *out_x, int *out_y, char kind, int radius)
{
    double best = 0.5; // порог обнаружения цели
    *out_x = cx; *out_y = cy;
    int dim = s->field_dim;

    for (int dx = -radius; dx <= radius; dx++) // сканирование квадрата
    {
        for (int dy = -radius; dy <= radius; dy++)
        {
            if (dx == 0 && dy == 0) continue; // своя позиция не интересует

            int tx = (cx + dx % dim + dim) % dim; // X на торе
            int ty = (cy + dy % dim + dim) % dim; // Y на торе

            double val = (kind == 'b') ? s->map[tx][ty].biomass : s->map[tx][ty].hares;
            if (val > best)
            {
                best = val;
                *out_x = tx;
                *out_y = ty;
            }
        }
    }
}

int main(int argc, char **argv) // процесс зайцев
{
    if (argc < 2) exit(1);

    int sid = atoi(argv[1]);
    SimShared *s = (SimShared *)shmat(sid, NULL, 0);
    srand(time(NULL) ^ getpid()); // уникальный сид

    while (s->active)
    {
        int dim = s->field_dim;
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                if (s->map[i][j].hares < 1.0) { s->map[i][j].hares = 0; continue; } // вымирание микро-популяции

                double h = s->map[i][j].hares;
                double b = s->map[i][j].biomass;

                double repro = h * b * s->hare_repro; // размножение от корма (Вольтерра)
                double loss = h * s->hare_decay; // естественная убыль

                double consumed = h * s->hare_consumption; // съеденная растительность
                s->map[i][j].biomass -= consumed;
                if (s->map[i][j].biomass < 0) s->map[i][j].biomass = 0;

                double next_h = h + repro - loss;
                if (next_h < 1.0) next_h = 0;
                if (next_h > FAUNA_CAP) next_h = FAUNA_CAP; // ограничение плотности
                s->map[i][j].hares = next_h;

                if (next_h >= 1.0) // миграционное поведение
                {
                    int tx, ty;
                    locate_best_tile(s, i, j, &tx, &ty, 'b', 2); // поиск корма в радиусе 2

                    int nx = i; int ny = j;

                    if ((tx != i || ty != j) && s->map[tx][ty].biomass >= 1.0)
                    {
                        // кратчайший вектор на торе
                        int dx = tx - i;
                        if (dx > dim / 2) dx -= dim; else if (dx < -dim / 2) dx += dim;

                        int dy = ty - j;
                        if (dy > dim / 2) dy -= dim; else if (dy < -dim / 2) dy += dim;

                        if (dx > 0) nx = (i + 1) % dim; else if (dx < 0) nx = (i - 1 + dim) % dim;
                        if (dy > 0) ny = (j + 1) % dim; else if (dy < 0) ny = (j - 1 + dim) % dim;
                    }
                    else // стохастическое блуждание
                    {
                        int rx = rand() % 3 - 1; int ry = rand() % 3 - 1;
                        nx = (i + rx + dim) % dim; ny = (j + ry + dim) % dim;
                    }

                    if (nx != i || ny != j) // фактический переход
                    {
                        double flow = next_h * s->hare_migration;
                        s->map[nx][ny].hares += flow;
                        s->map[i][j].hares -= flow;
                    }
                }
            }
        }
        usleep(400000); // пауза 0.4с
    }

    shmdt(s);
    exit(0);
}
