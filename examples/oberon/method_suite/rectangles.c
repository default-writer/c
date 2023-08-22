#include "rectangles.h"
#include "config.h"
#include "figures.h"
#include "new.h"
#include <assert.h>

rectangle_type_pointer rectangle_type_instance;

extern figure_type_pointer figure_type_instance;

rectangle_pointer rectangle_new() {
    rectangle_pointer pointer = calloc(1, sizeof(rectangle));
    pointer->type = *rectangle_type_instance;
    return pointer;
}

void rectangle_delete(rectangle_pointer pointer) {
    free(pointer);
}

void rectangle_draw(rectangle_pointer r) {
    figure_type_pointer f = (figure_type_pointer)&r->type;
    figure_data_pointer f_data = (figure_data_pointer)&r->data;
    f->draw(f_data);
    r->type.draw(&r->data);
}

void rectangle_move(rectangle_pointer r, int dx, int dy) {
    figure_type_pointer f = (figure_type_pointer)&r->type;
    figure_data_pointer f_data = (figure_data_pointer)&r->data;
    f->move(f_data, dx, dy);
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

static void __attribute__((constructor)) rectangle_init() {
    rectangle_type_instance = calloc(1, sizeof(rectangle_type));
    rectangle_type_instance->figure = *figure_type_instance;
    rectangle_type_instance->draw = rectangle_type_draw;
    rectangle_type_instance->move = rectangle_type_move;
}

static void __attribute__((destructor)) rectangle_free() {
    free(rectangle_type_instance);
}
