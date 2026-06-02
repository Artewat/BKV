#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_STARS 200 // Максимальное одновременно видимое число звезд
#define PI 3.14159265358979323846

typedef struct {
    int x, y;
    int r_outer;
    int r_inner;
    int num_rays;
    unsigned long color; // Запоминаем цвет, чтобы при необходимости перерисовать
} Star;

Star stars[MAX_STARS];
int star_count = 0;

// Математическая генерация точек звезды
void generate_star_points(int x, int y, int num_rays, int r_outer, int r_inner, XPoint *points) {
    int total_points = num_rays * 2;
    double angle_step = PI / num_rays;
    double current_angle = -PI / 2;

    for (int i = 0; i < total_points; i++) {
        int r = (i % 2 == 0) ? r_outer : r_inner;
        points[i].x = x + (short)(r * cos(current_angle));
        points[i].y = y + (short)(r * sin(current_angle));
        current_angle += angle_step;
    }
    points[total_points] = points[0];
}

// Функция стирания конкретной звезды черным цветом
void erase_star(Display *display, Window window, GC gc, Star *star) {
    int total_points = star->num_rays * 2;
    XPoint *points = malloc(sizeof(XPoint) * (total_points + 1));
    generate_star_points(star->x, star->y, star->num_rays, star->r_outer, star->r_inner, points);
    
    // Устанавливаем черный цвет (цвет фона)
    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));
    XFillPolygon(display, window, gc, points, total_points, Complex, CoordModeOrigin);
    
    free(points);
}

// Проверка пересечений с удалением перекрытых звезд
void handle_intersections(Display *display, Window window, GC gc, int new_x, int new_y, int new_r) {
    for (int i = 0; i < star_count; i++) {
        int dx = new_x - stars[i].x;
        int dy = new_y - stars[i].y;
        double distance = sqrt(dx * dx + dy * dy);
        
        // Если новая звезда задевает старую (с запасом в 2 пикселя)
        if (distance < (new_r + stars[i].r_outer + 2)) {
            // 1. Стираем старую звезду с экрана
            erase_star(display, window, gc, &stars[i]);
            
            // 2. Удаляем её из массива (сдвигаем хвост массива на её место)
            for (int j = i; j < star_count - 1; j++) {
                stars[j] = stars[j + 1];
            }
            star_count--;
            i--; // Уменьшаем индекс, чтобы проверить элемент, вставший на это место
        }
    }
}

int main() {
    srand(time(NULL));

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Не удалось открыть X-дисплей\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    int width = 800;
    int height = 600;

    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen),
        100, 100, width, height, 1,
        BlackPixel(display, screen), BlackPixel(display, screen)
    );

    XStoreName(display, window, "X11 Infinite Overwriting Stars");
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, NULL);
    XEvent event;
    
    bool running = true;
    bool window_ready = false;

    printf("Запуск бесконечной генерации. Для выхода нажмите любую клавишу в окне...\n");

    while (running) {
        // Обработка системных событий
        while (XPending(display) > 0) {
            XNextEvent(display, &event);
            if (event.type == KeyPress) {
                running = false;
            }
            if (event.type == Expose) {
                window_ready = true;
            }
        }

        if (window_ready) {
            // Диапазон размеров звезд (теперь они всегда разнообразные)
            int r_outer = (rand() % 45) + 15; // Радиус от 15 до 60
            int r_inner = r_outer / 2;
            int num_rays = (rand() % 6) + 5;  // От 5 до 10 лучей

            int x = (rand() % (width - r_outer * 2)) + r_outer;
            int y = (rand() % (height - r_outer * 2)) + r_outer;

            // Находим все пересекающиеся старые звезды, стираем их черным и удаляем из базы
            handle_intersections(display, window, gc, x, y, r_outer);

            // Защита от переполнения массива (если экран совсем пустой, чего не случится)
            if (star_count < MAX_STARS) {
                int total_points = num_rays * 2;
                XPoint *points = malloc(sizeof(XPoint) * (total_points + 1));
                generate_star_points(x, y, num_rays, r_outer, r_inner, points);

                // Генерируем случайный цвет
                unsigned long color = ((rand() % 200 + 55) << 16) | 
                                      ((rand() % 200 + 55) << 8)  | 
                                      (rand() % 200 + 55);
                
                // Рисуем новую звезду поверх очищенного места
                XSetForeground(display, gc, color);
                XFillPolygon(display, window, gc, points, total_points, Complex, CoordModeOrigin);
                
                free(points);

                // Сохраняем параметры новой звезды
                stars[star_count].x = x;
                stars[star_count].y = y;
                stars[star_count].r_outer = r_outer;
                stars[star_count].r_inner = r_inner;
                stars[star_count].num_rays = num_rays;
                stars[star_count].color = color;
                star_count++;
            }

            // Синхронизируем картинку и делаем небольшую паузу для плавности анимации
            XSync(display, False);
            usleep(25000); // 25 миллисекунд (изменяйте для ускорения/замедления)
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}