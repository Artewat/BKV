#ifndef SIM_COMMON_H // защита от повторного подключения
#define SIM_COMMON_H

#define FIELD_MAX 100 // предельный размер квадратного поля
#define GRASS_CAP 100.0 // верхний предел биомассы растений
#define FAUNA_CAP 100.0 // верхний предел особей в одной ячейке

typedef struct // ячейка игрового поля
{
    double biomass; // растительность
    double hares;   // популяция зайцев
    double wolves;  // популяция волков
} Tile;

typedef struct // блок разделяемой памяти для межпроцессного взаимодействия
{
    Tile map[FIELD_MAX][FIELD_MAX]; // сетка с запасом по размеру

    int field_dim; // актуальный размер стороны поля (устанавливается пользователем)

    double biomass_growth, biomass_spread; // параметры растений
    double hare_repro, hare_migration, hare_consumption; // параметры зайцев
    double wolf_repro, wolf_migration, wolf_consumption; // параметры волков

    double hare_decay; // фоновая смертность зайцев
    double wolf_decay; // фоновая смертность волков

    int active; // индикатор работоспособности симуляции
} SimShared;

#endif
