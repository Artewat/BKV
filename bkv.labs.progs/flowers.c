#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_FLOWERS 100
#define M_PI 3.14159265358979323846
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

typedef struct {
    int x, y;
    int diameter;
    int petals;
    unsigned long petal_color;
} Flower;

Flower flowers[MAX_FLOWERS];
int flower_count = 0;

unsigned long coral, thistle, green, yellow, HotPink, PeachPuff1, MistyRose2, white_pixel;
unsigned long petal_colors[10];
int NUM_PETAL_COLORS = 0;

Display *dis = NULL;
int screen;
Window win;
GC gc;

void fillCircle(int x0, int y0, int radius);
void draw_flower(Flower *f);
void get_colors();
int is_overlapping(Flower *f1, Flower *f2);
int is_within_bounds(Flower *f, int win_width, int win_height);
void add_flower(int win_width, int win_height);
void init_x();
void redraw();
void cleanup();

void fillCircle(int x0, int y0, int radius) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    XDrawLine(dis, win, gc, x0 - radius, y0, x0 + radius, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        XDrawLine(dis, win, gc, x0 - x, y0 + y, x0 + x, y0 + y);
        XDrawLine(dis, win, gc, x0 - x, y0 - y, x0 + x, y0 - y);
        XDrawLine(dis, win, gc, x0 - y, y0 + x, x0 + y, y0 + x);
        XDrawLine(dis, win, gc, x0 - y, y0 - x, x0 + y, y0 - x);
    }
}

void draw_flower(Flower *f) {
    if (!f) return;
    
    int r_center = f->diameter / 8;
    int center_x = f->x;
    int center_y = f->y;

    XSetForeground(dis, gc, yellow);
    fillCircle(center_x, center_y, r_center);

    XSetForeground(dis, gc, f->petal_color);
    int num_petals = f->petals;
    int petal_radius = r_center;

    for (int i = 0; i < num_petals; i++) {
        double angle = 2 * M_PI * i / num_petals;
        int petal_x = center_x + (3 * r_center) * cos(angle);
        int petal_y = center_y + (3 * r_center) * sin(angle);
        fillCircle(petal_x, petal_y, petal_radius);
    }
}

void get_colors() {
    XColor tmp;
    Colormap cmap = DefaultColormap(dis, screen);

    XParseColor(dis, cmap, "blue", &tmp);
    XAllocColor(dis, cmap, &tmp);
    coral = tmp.pixel;
    petal_colors[NUM_PETAL_COLORS++] = coral;

    XParseColor(dis, cmap, "HotPink", &tmp);
    XAllocColor(dis, cmap, &tmp);
    HotPink = tmp.pixel;
    petal_colors[NUM_PETAL_COLORS++] = HotPink;

    XParseColor(dis, cmap, "thistle", &tmp);
    XAllocColor(dis, cmap, &tmp);
    thistle = tmp.pixel;
    petal_colors[NUM_PETAL_COLORS++] = thistle;

    XParseColor(dis, cmap, "PeachPuff1", &tmp);
    XAllocColor(dis, cmap, &tmp);
    PeachPuff1 = tmp.pixel;
    petal_colors[NUM_PETAL_COLORS++] = PeachPuff1;

    XParseColor(dis, cmap, "MistyRose2", &tmp);
    XAllocColor(dis, cmap, &tmp);
    MistyRose2 = tmp.pixel;
    petal_colors[NUM_PETAL_COLORS++] = MistyRose2;

    XParseColor(dis, cmap, "white", &tmp);
    XAllocColor(dis, cmap, &tmp);
    white_pixel = tmp.pixel;
    petal_colors[NUM_PETAL_COLORS++] = white_pixel;

    XParseColor(dis, cmap, "yellow", &tmp);
    XAllocColor(dis, cmap, &tmp);
    yellow = tmp.pixel;

    XParseColor(dis, cmap, "green", &tmp);
    XAllocColor(dis, cmap, &tmp);
    green = tmp.pixel;
}

int is_overlapping(Flower *f1, Flower *f2) {
    if (!f1 || !f2) return 0;
    
    int dx = f1->x - f2->x;
    int dy = f1->y - f2->y;
    long distance_sq = (long)dx * dx + (long)dy * dy;
    int r1 = f1->diameter / 2;
    int r2 = f2->diameter / 2;
    long sum_radii = (long)r1 + r2;
    return distance_sq < sum_radii * sum_radii;
}

int is_within_bounds(Flower *f, int win_width, int win_height) {
    if (!f) return 0;
    
    int r = f->diameter / 2;
    return (f->x >= r) && (f->x + r < win_width) && (f->y >= r) && (f->y + r < win_height);
}

void add_flower(int win_width, int win_height) {
    int max_attempts = 100;
    int added = 0;

    for (int attempt = 0; attempt < max_attempts && !added; attempt++) {
        Flower new_flower;
        new_flower.diameter = 20 + rand() % 41;
        new_flower.petals = 5;
        new_flower.petal_color = petal_colors[rand() % NUM_PETAL_COLORS];
        int r = new_flower.diameter / 2;

        new_flower.x = r + rand() % (win_width - 2 * r);
        new_flower.y = r + rand() % (win_height - 2 * r);

        if (!is_within_bounds(&new_flower, win_width, win_height)) {
            continue;
        }

        int overlap = 0;
        for (int i = 0; i < flower_count; i++) {
            if (is_overlapping(&new_flower, &flowers[i])) {
                overlap = 1;
                break;
            }
        }

        if (!overlap) {
            if (flower_count < MAX_FLOWERS) {
                flowers[flower_count++] = new_flower;
                added = 1;
            } else {
                // Shift array left to make space
                for (int i = 0; i < flower_count - 1; i++) {
                    flowers[i] = flowers[i + 1];
                }
                flower_count--;
            }
        }
    }

    if (!added && flower_count > 0) {
        // Shift array left to make space
        for (int i = 0; i < flower_count - 1; i++) {
            flowers[i] = flowers[i + 1];
        }
        flower_count--;
        
        // Try again
        add_flower(win_width, win_height);
    }
}

void init_x() {
    unsigned long black, white;
    dis = XOpenDisplay(NULL);
    if (!dis) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(dis);
    black = BlackPixel(dis, screen);
    white = WhitePixel(dis, screen);

    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 
                             WINDOW_WIDTH, WINDOW_HEIGHT, 5, black, white);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);

    gc = XCreateGC(dis, win, 0, 0);
    if (!gc) {
        fprintf(stderr, "Cannot create GC\n");
        cleanup();
        exit(1);
    }

    XSetBackground(dis, gc, black);
    XSetForeground(dis, gc, white);

    get_colors();
    XSetWindowBackground(dis, win, green);
    XClearWindow(dis, win);
    XMapWindow(dis, win);
    XFlush(dis);
}

void redraw() {
    if (!dis) return;
    
    XClearWindow(dis, win);
    for (int i = 0; i < flower_count; i++) {
        draw_flower(&flowers[i]);
    }
}

void cleanup() {
    if (gc) XFreeGC(dis, gc);
    if (win) XDestroyWindow(dis, win);
    if (dis) XCloseDisplay(dis);
}

int main() {
    srand(time(NULL));
    init_x();

    XEvent event;
    KeySym key;
    char text[255];

    while (1) {
        XNextEvent(dis, &event);

        switch (event.type) {
            case Expose:
                if (event.xexpose.count == 0) {
                    redraw();
                }
                break;
                
            case KeyPress:
                if (XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
                    if (text[0] == 'q') {
                        cleanup();
                        exit(0);
                    }
                }
                break;
                
            case ButtonPress:
                if (event.xbutton.button == 1) {
                    add_flower(WINDOW_WIDTH, WINDOW_HEIGHT);
                    redraw();
                }
                break;
        }
    }
}