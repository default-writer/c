#include "figures.h"
#include "config.h"
#include "new.h"

typedef struct figure* figure_pointer;

typedef struct figure {
    figure_type_pointer type;
    figure_data data;
} figure;

figure_type_pointer figure_type_instance;

void figure_init(figure_type* type, figure_data* data) {
    *type = *figure_type_instance;
    data->version = 1;
}

figure_pointer figure_new() {
    figure_pointer pointer = calloc(1, sizeof(figure));
    figure_init(pointer->type, &pointer->data);
    return pointer;
}

static void figure_draw(figure_data_pointer f) {
    printf("figure draw v%d\n", f->version);
}

static void figure_move(figure_data_pointer f, int dx, int dy) {
    printf("figure move v%d\n", f->version);
}

static void __attribute__((constructor)) figure_type_init() {
    figure_type_instance = calloc(1, sizeof(figure_type));
    figure_type_instance->draw = figure_draw;
    figure_type_instance->move = figure_move;
}

static void __attribute__((destructor)) figure_type_free() {
    free(figure_type_instance);
}
