#include <stdlib.h> // exit - коды возврата
#include <unistd.h> // usleep - задержки
#include <sys/shm.h> // shmat - подключение к общей памяти
#include "common.h" // общие определения

int main(int argc, char **argv) // процесс растительности
{
    if (argc < 2) exit(1); // требуется ID сегмента

    int sid = atoi(argv[1]); // парсинг аргумента
    SimShared *s = (SimShared *)shmat(sid, NULL, 0); // подключение к сегменту

    double buf[FIELD_MAX][FIELD_MAX]; // буфер диффузии

    while (s->active)
    {
        int dim = s->field_dim; // локальная копия размера

        for (int i = 0; i < dim; i++) // снимок текущего состояния
        { for (int j = 0; j < dim; j++) { buf[i][j] = s->map[i][j].biomass; } }

        for (int i = 0; i < dim; i++) // расчёт прироста и миграции (тороидальная топология)
        {
            for (int j = 0; j < dim; j++)
            {
                double cur = buf[i][j];
                double grow = (cur > 0) ? s->biomass_growth * cur * (1.0 - cur / GRASS_CAP) : 0; // S-образная кривая

                // соседи с учётом замыкания краёв
                int up = (i - 1 + dim) % dim;
                int down = (i + 1) % dim;
                int left = (j - 1 + dim) % dim;
                int right = (j + 1) % dim;

                // усреднённое значение вокруг
                double total = buf[up][j] + buf[down][j] + buf[i][left] + buf[i][right];
                double mean = total / 4.0;
                double spread = s->biomass_spread * (mean - cur); // перенос семян

                s->map[i][j].biomass += (grow + spread); // накопление
                if (s->map[i][j].biomass > GRASS_CAP) s->map[i][j].biomass = GRASS_CAP;
                if (s->map[i][j].biomass < 1.0) s->map[i][j].biomass = 0;
            }
        }
        usleep(500000); // пауза 0.5с
    }

    shmdt(s); // отключение
    exit(0);
}
