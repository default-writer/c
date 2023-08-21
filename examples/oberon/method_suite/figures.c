#include "figures.h"
#include "config.h"
#include "new.h"

static interface figure_i;

void figure_init(figure f, interface derived) {
#if defined(INHERITANCE) || defined(EMBEDDED_INHERITANCE)
    f->version = 1; // value inheritance
    derived->i = figure_i; // interface inheritance
    f->i = derived;
#endif
}

static void figure_draw(figure f) {
    printf("figure draw v%d\n", f->version);
}

static void figure_move(figure f, int dx, int dy) {
    printf("figure move v%d\n", f->version);
}

static void __attribute__((constructor)) rectangle_init() {
    figure_i = calloc(1, sizeof(interface_desc));
    figure_i->draw = figure_draw;
    figure_i->move = figure_move;
}

static void __attribute__((destructor)) rectangle_free() {
    free(figure_i);
}
