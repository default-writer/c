#include "type.h"
#include "../new.h"

rectangle_type_pointer rectangle;

typedef struct rectangle_data {
    int version;
    int x, y, w, h;
} *rectangle_data_pointer, rectangle_data;

typedef struct rectangle_object {
    rectangle_data_pointer data;
    figure_pointer figure;
} rectangle_object;

static rectangle_pointer rectangle_new() {
    rectangle_pointer ptr = calloc(1, sizeof(rectangle_object) + sizeof(rectangle_data));
    ptr->data = (rectangle_data_pointer)(ptr + 1);
    return ptr;
}

static rectangle_pointer rectangle_create() {
    rectangle_pointer ptr = rectangle_new();
    ptr->figure = figure->create();
    ptr->data->w = 42;
    ptr->data->version = 3;
    printf("reactangle create v%d\n", ptr->data->version);
    return ptr;
}

static void rectangle_destroy(rectangle_pointer ptr) {
    figure->destroy(ptr->figure);
    printf("reactangle destroy v%d\n", ptr->data->version);
    free(ptr);
}

static void rectangle_type_draw(rectangle_pointer ptr) {
    figure->draw(ptr->figure);
    int x = ptr->data->x;
    int y = ptr->data->y;
    int w = ptr->data->w;
    int h = ptr->data->h;
    printf("rectangle draw[%d,%d,%d,%d]\n", x, y, w, h);
}

static void rectangle_type_move(rectangle_pointer r, int dx, int dy) {
    r->data->x += dx;
    r->data->y += dy;
    printf("rectangle move[%d,%d,%d,%d]\n", r->data->x, r->data->y, r->data->w, r->data->h);
}

static void __attribute__((constructor)) rectangle_type_init() {
    rectangle = calloc(1, sizeof(rectangle_type));
    rectangle->figure = *figure;
    rectangle->draw = rectangle_type_draw;
    rectangle->move = rectangle_type_move;
    rectangle->create = rectangle_create;
    rectangle->destroy = rectangle_destroy;
}

static void __attribute__((destructor)) rectangle_type_free() {
    free(rectangle);
}