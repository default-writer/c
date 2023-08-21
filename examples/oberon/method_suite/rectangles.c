#include "rectangles.h"
#include "config.h"
#include "new.h"
#include <assert.h>

static interface rectangle_i;

extern void figure_init(figure f, interface i);

void rectangle_new(rectangle* r) {
    rectangle new_r = calloc(1, sizeof(rectangle_desc));
    figure f = (figure)new_r;
    figure_init(f, rectangle_i);
    *r = new_r;
}

static void rectangle_draw(figure f) {
    rectangle r = (rectangle)f;
    int x = r->x;
    int y = r->y;
    int w = r->w;
    int h = r->h;
#if defined(INHERITANCE)
    interface i = f->i->i;
    if (i && i->draw) {
        i->draw(f); // interface inheritance
    }
#endif
    printf("rectangle draw[%d,%d,%d,%d]\n", x, y, w, h);
}

static void rectangle_move(figure f, int dx, int dy) {
    rectangle r = (rectangle)f;
    r->x += dx;
    r->y += dy;
#if defined(INHERITANCE)
    interface i = f->i->i;
    if (i && i->move) {
        i->move(f, dx, dy); // interface inheritance
    }
#endif
    printf("rectangle move[%d,%d,%d,%d]\n", r->x, r->y, r->w, r->h);
}

static void __attribute__((constructor)) rectangle_init() {
    rectangle_i = calloc(1, sizeof(interface_desc));
    rectangle_i->draw = rectangle_draw;
    rectangle_i->move = rectangle_move;
}

static void __attribute__((destructor)) rectangle_free() {
    free(rectangle_i);
}
