#include <stdio.h> // printf - вывод в консоль
#include <stdlib.h> // exit - завершение процесса
#include <string.h> // memset - обнуление памяти
#include <unistd.h> // fork, usleep, execlp - управление процессами
#include <sys/wait.h> // waitpid - ожидание дочерних процессов
#include <sys/ipc.h> // IPC_PRIVATE - ключи для IPC
#include <sys/shm.h> // shmget, shmat, shmdt, shmctl - API разделяемой памяти
#include <errno.h> // errno - диагностика ошибок

#include <FL/Fl.H> // основа FLTK
#include <FL/Fl_Double_Window.H> // окно без мерцания
#include <FL/Fl_Button.H> // интерактивные кнопки
#include <FL/Fl_Value_Input.H> // числовой ввод
#include <FL/Fl_Box.H> // текстовые метки
#include <FL/fl_draw.H> // палитра и рисование
#include "common.h" // общие типы и константы

SimShared *sim = NULL; // дескриптор общего сегмента
int shm_id; // ID сегмента IPC
pid_t proc_grass = 0, proc_rabbit = 0, proc_wolf = 0; // идентификаторы актёров
int tile_px = 25; // габарит одной ячейки на экране

Fl_Button **tiles = NULL; // плоский массив визуальных ячеек
Fl_Value_Input *val_bg, *val_hr, *val_hw, *val_mr, *val_mp, *val_mw, *val_cr, *val_cw, *inp_dim, *inp_px; // регуляторы
Fl_Button *btn_start; // пуск модели

// освобождение ресурсов и остановка дочерних процессов
void cb_exit_all(Fl_Widget* w, void* data)
{
    if (sim) sim->active = 0; // сигнал завершения для актёров

    // отправка сигнала и ожидание потомков
    if (proc_grass > 0) { kill(proc_grass, SIGTERM); waitpid(proc_grass, NULL, 0); }
    if (proc_rabbit > 0) { kill(proc_rabbit, SIGTERM); waitpid(proc_rabbit, NULL, 0); }
    if (proc_wolf > 0) { kill(proc_wolf, SIGTERM); waitpid(proc_wolf, NULL, 0); }

    if (sim) shmdt(sim); // отключение от сегмента
    shmctl(shm_id, IPC_RMID, NULL); // ликвидация сегмента
    exit(0); // выход из приложения
}

// периодическая перерисовка поля (циклический таймер FLTK)
static void update_gui(void*)
{
    if (!sim || !sim->active) return;

    // считывание GUI-параметров 0.1-100.0 и нормализация
    sim->biomass_growth = val_bg->value() / 100.0; sim->biomass_spread = val_mp->value() / 100.0;
    sim->hare_repro = val_hr->value() / 100.0; sim->hare_migration = val_mr->value() / 100.0; sim->hare_consumption = val_cr->value() / 100.0;
    sim->wolf_repro = val_hw->value() / 100.0; sim->wolf_migration = val_mw->value() / 100.0; sim->wolf_consumption = val_cw->value() / 100.0;

    int dim = sim->field_dim;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            double b = sim->map[i][j].biomass;
            double h = sim->map[i][j].hares;
            double w = sim->map[i][j].wolves;

            // формирование RGB-каналов
            int r = (w >= 1.0) ? 80 + (int)(w * 5.0) : 0;
            int g = (b >= 1.0) ? 50 + (int)(b * 2.0) : 0;
            int bl = (h >= 1.0) ? 80 + (int)(h * 5.0) : 0;
            if (r > 255) r = 255; if (g > 255) g = 255; if (bl > 255) bl = 255; // ограничение яркости

            int idx = i * dim + j; // линейный индекс
            if (r == 0 && g == 0 && bl == 0) tiles[idx]->color(FL_BLACK);
            else tiles[idx]->color(fl_rgb_color(r, g, bl));
            tiles[idx]->redraw(); // запрос перерисовки
        }
    }
    Fl::repeat_timeout(0.1, update_gui); // планирование следующего кадра
}

// реакция на нажатие мышью по ячейке карты
void cb_grid_click(Fl_Widget* w, void* data)
{
    long idx = (long)data;
    int dim = sim->field_dim;
    int i = idx / dim; // восстановление строки
    int j = idx % dim; // восстановление столбца

    int btn = Fl::event_button(); // какая клавиша мыши сработала
    if (btn == FL_LEFT_MOUSE) sim->map[i][j].biomass += 40.0;
    else if (btn == FL_RIGHT_MOUSE) sim->map[i][j].hares += 20.0;
    else if (btn == FL_MIDDLE_MOUSE) sim->map[i][j].wolves += 10.0;
}

// инициализация мира и порождение процессов-акторов
static void cb_GO(Fl_Widget*, void*)
{
    if (sim->active) return; // блокировка повторного старта
    sim->field_dim = (int)inp_dim->value(); // габарит решётки
    tile_px = (int)inp_px->value(); // размер тайла

    // блокировка органов управления
    inp_dim->deactivate();
    inp_px->deactivate();
    btn_start->deactivate();

    sim->hare_decay = 0.015; sim->wolf_decay = 0.025; // установка фоновой смертности
    sim->active = 1;
    char sid_buf[20]; sprintf(sid_buf, "%d", shm_id); // строковое представление ID

    // запуск дочерних исполнителей
    proc_grass = fork(); if (proc_grass == 0) { execlp("./grass", "grass", sid_buf, NULL); exit(1); }
    proc_rabbit = fork(); if (proc_rabbit == 0) { execlp("./rabbit", "rabbit", sid_buf, NULL); exit(1); }
    proc_wolf = fork(); if (proc_wolf == 0) { execlp("./wolf", "wolf", sid_buf, NULL); exit(1); }

    int dim = sim->field_dim;
    int cs = tile_px;
    Fl_Double_Window *win_field = new Fl_Double_Window(dim * cs, dim * cs, "Игровое поле");
    win_field->callback(cb_exit_all); // закрытие = выход

    tiles = new Fl_Button*[dim * dim]; // резервирование указателей
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            int idx = i * dim + j;
            tiles[idx] = new Fl_Button(j * cs, i * cs, cs, cs, "");
            tiles[idx]->callback(cb_grid_click, (void*)(long)idx); // привязка клика
            tiles[idx]->box(FL_FLAT_BOX);
            tiles[idx]->color(FL_BLACK); // стартовый тон
        }
    }
    win_field->show();
    Fl::add_timeout(0.1, update_gui); // активация рендер-цикла
}

int main(int argc, char **argv) // входная точка контроллера
{
    shm_id = shmget(IPC_PRIVATE, sizeof(SimShared), IPC_CREAT | 0666); // создание общего сегмента
    if (shm_id < 0) { perror("Ошибка shmget"); exit(1); }
    sim = (SimShared *)shmat(shm_id, NULL, 0666); // маппинг в адресное пространство
    memset(sim, 0, sizeof(SimShared)); // обнуление

    Fl_Double_Window* w = new Fl_Double_Window(320, 420, "Панель управления");
    w->box(FL_PLASTIC_DOWN_BOX);
    w->callback(cb_exit_all); // реакция на закрытие

    // параметры решётки
    Fl_Box* l_sys = new Fl_Box(10, 10, 300, 20, "ПАРАМЕТРЫ СИСТЕМЫ");
    inp_dim = new Fl_Value_Input(90, 40, 50, 25, "Размер ");
    inp_dim->step(1); inp_dim->precision(0); inp_dim->bounds(5, 100); inp_dim->value(20);
    inp_px = new Fl_Value_Input(230, 40, 50, 25, "Пикс. ");
    inp_px->step(1); inp_px->precision(0); inp_px->bounds(10, 50); inp_px->value(25);

    // биологические константы
    Fl_Box* l_bio = new Fl_Box(10, 80, 300, 20, "БИО-ПАРАМЕТРЫ");
    Fl_Box* l1 = new Fl_Box(90, 110, 50, 20, "Рост");
    Fl_Box* l2 = new Fl_Box(160, 110, 50, 20, "Мигр.");
    Fl_Box* l3 = new Fl_Box(230, 110, 50, 20, "Поед.");

    val_hr = new Fl_Value_Input(90, 140, 50, 25, "Зайцы ");
    val_hr->step(0.1); val_hr->bounds(0.1, 100.0); val_hr->value(5.0);
    val_mr = new Fl_Value_Input(160, 140, 50, 25);
    val_mr->step(0.1); val_mr->bounds(0.1, 100.0); val_mr->value(80.0);
    val_cr = new Fl_Value_Input(230, 140, 50, 25);
    val_cr->step(0.1); val_cr->bounds(0.1, 100.0); val_cr->value(2.0);

    val_bg = new Fl_Value_Input(90, 180, 50, 25, "Трава ");
    val_bg->step(0.1); val_bg->bounds(0.1, 100.0); val_bg->value(20.0);
    val_mp = new Fl_Value_Input(160, 180, 50, 25);
    val_mp->step(0.1); val_mp->bounds(0.1, 100.0); val_mp->value(10.0);

    val_hw = new Fl_Value_Input(90, 220, 50, 25, "Волки ");
    val_hw->step(0.1); val_hw->bounds(0.1, 100.0); val_hw->value(5.0);
    val_mw = new Fl_Value_Input(160, 220, 50, 25);
    val_mw->step(0.1); val_mw->bounds(0.1, 100.0); val_mw->value(90.0);
    val_cw = new Fl_Value_Input(230, 220, 50, 25);
    val_cw->step(0.1); val_cw->bounds(0.1, 100.0); val_cw->value(10.0);

    Fl_Box* info = new Fl_Box(10, 260, 300, 100,
		"Размер поля: 5-100, пиксели: 10-50.\n"
		"Коэффициенты: 0.1-100.0\n"
        "ЛКМ - Растения, ПКМ - Зайцы, СКМ - Волки.\n\n"
        "Настройки подхватываются на лету.\n"
        "Закрытие окна останавливает симуляцию.");
    info->box(FL_ENGRAVED_BOX);
    info->labelsize(11);

    btn_start = new Fl_Button(60, 370, 80, 30, "Пуск");
    btn_start->box(FL_PLASTIC_DOWN_BOX);
    btn_start->callback((Fl_Callback*)cb_GO); // обработчик запуска

    Fl_Button* btn_quit = new Fl_Button(180, 370, 80, 30, "Стоп");
    btn_quit->callback(cb_exit_all); // обработчик остановки
    w->end(); w->show(argc, argv);

    return Fl::run(); // главный цикл обработки событий
}
