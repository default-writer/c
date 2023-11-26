#include "rectangles.h"
#include "figures.h"
#include "new.h"

typedef struct rectangle {
    rectangle_type_pointer type;
    rectangle_data_pointer data;
} rectangle;

rectangle_type_pointer rectangle_type_instance;

extern figure_type_pointer figure_type_instance;
extern void figure_init(figure_data_pointer data);

void rectangle_init(rectangle_data_pointer data) {
    data->w = 42;
    figure_init((figure_data_pointer)data);
}

rectangle_pointer rectangle_new(void) {
    rectangle_pointer pointer = calloc(1, sizeof(rectangle) + sizeof(rectangle_data));
    pointer->data = (rectangle_data_pointer)(pointer + 1);
    pointer->type = rectangle_type_instance;
    rectangle_init(pointer->data);
    return pointer;
}

void rectangle_delete(rectangle_pointer pointer) {
    free(pointer);
}

void rectangle_draw(rectangle_pointer r) {
    figure_type_pointer type = (figure_type_pointer)r->type;
    figure_data_pointer data = (figure_data_pointer)r->data;
    type->draw(data);
}

void rectangle_move(rectangle_pointer r, int dx, int dy) {
    r->type->move(r->data, dx, dy);
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

static void __attribute__((constructor)) rectangle_type_init(void) {
    rectangle_type_instance = calloc(1, sizeof(rectangle_type));
    rectangle_type_instance->figure = *figure_type_instance;
    rectangle_type_instance->draw = rectangle_type_draw;
    rectangle_type_instance->move = rectangle_type_move;
}

static void __attribute__((destructor)) rectangle_type_free(void) {
    free(rectangle_type_instance);
}
