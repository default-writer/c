#include "rectangles.h"
#include "config.h"
#include "figures.h"
#include "new.h"
#include <assert.h>

typedef struct rectangle_type* rectangle_type_pointer;
typedef struct rectangle_data* rectangle_data_pointer;

typedef struct rectangle_type {
    figure_type figure;
    void (*draw)(rectangle_data_pointer f);
    void (*move)(rectangle_data_pointer f, int dx, int dy);
} rectangle_type;

typedef struct rectangle_data {
    figure_data figure;
    int x, y, w, h;
} rectangle_data;

typedef struct rectangle {
    rectangle_type type;
    rectangle_data data;
} rectangle;

rectangle_type_pointer rectangle_type_instance;

extern figure_type_pointer figure_type_instance;
extern void figure_init(figure_type* type, figure_data* data);

void rectangle_init(rectangle_type_pointer type, rectangle_data_pointer data) {
    *type = *rectangle_type_instance;
    data->w = 42;
    figure_init((figure_type_pointer)type, (figure_data_pointer)data);
}

rectangle_pointer rectangle_new() {
    rectangle_pointer pointer = calloc(1, sizeof(rectangle));
    rectangle_init(&pointer->type, &pointer->data);
    return pointer;
}

void rectangle_delete(rectangle_pointer pointer) {
    free(pointer);
}

void rectangle_draw(rectangle_pointer r) {
    figure_type_pointer f = (figure_type_pointer)r;
    f->draw((figure_data_pointer)&r->data);
}

void rectangle_move(rectangle_pointer r, int dx, int dy) {
    r->type.move(&r->data, dx, dy);
}

static void rectangle_type_draw(rectangle_data_pointer r) {
    int x = r->x;
    int y = r->y;
    int w = r->w;
    int h = r->h;
    printf("rectangle draw[%d,%d,%d,%d]\n", x, y, w, h);
}

static void rectangle_type_move(rectangle_data_pointer r, int dx, int dy) {
    r->x += dx;
    r->y += dy;
    printf("rectangle move[%d,%d,%d,%d]\n", r->x, r->y, r->w, r->h);
}

static void __attribute__((constructor)) rectangle_type_init() {
    rectangle_type_instance = calloc(1, sizeof(rectangle_type));
    rectangle_type_instance->figure = *figure_type_instance;
    rectangle_type_instance->draw = rectangle_type_draw;
    rectangle_type_instance->move = rectangle_type_move;
}

static void __attribute__((destructor)) rectangle_type_free() {
    free(rectangle_type_instance);
}
