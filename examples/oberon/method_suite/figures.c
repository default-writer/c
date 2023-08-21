#include "figures.h"

static interface i;

void figure_init(figure f, interface derived) {
    // f->value = 0x7fffffff;
    // derived->i = i;
    f->i = derived;
}

static void figure_draw(figure f) {
}

static void figure_move(figure f, int dx, int dy) {
}

static void __attribute__((constructor)) rectangle_init() {
    i = calloc(1, sizeof(interface_desc));
    i->draw = figure_draw;
    i->move = figure_move;
}

static void __attribute__((destructor)) rectangle_free() {
    free(i);
}
