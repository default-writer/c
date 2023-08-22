#include "figures.h"
#include "config.h"
#include "new.h"

figure_type_pointer figure_type_instance;

static void figure_draw(figure_data_pointer f) {
    printf("figure draw v%d\n", f->version);
}

static void figure_move(figure_data_pointer f, int dx, int dy) {
    printf("figure move v%d\n", f->version);
}

static void __attribute__((constructor)) figure_init() {
    figure_type_instance = calloc(1, sizeof(figure_type));
    figure_type_instance->draw = figure_draw;
    figure_type_instance->move = figure_move;
}

static void __attribute__((destructor)) figure_free() {
    free(figure_type_instance);
}
