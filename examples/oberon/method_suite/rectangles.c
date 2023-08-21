#include <stdio.h>
#include <stdlib.h>

#include "figures.h"
#include "rectangles.h"

static interface i;

extern void figure_init(figure f, interface i);

void rectangle_new(rectangle* r) {
    *r = calloc(1, sizeof(rectangle_desc));
    figure_init((figure)*r, i);
}

static void rectangle_draw(figure f) {
    rectangle r = (rectangle)f;
    printf("draw[%d,%d,%d,%d]\n", r->x, r->y, r->w, r->h);
}

static void rectangle_move(figure f, int dx, int dy) {
    rectangle r = (rectangle)f;
    r->x += dx;
    r->y += dy;
    printf("move[%d,%d,%d,%d]\n", r->x, r->y, r->w, r->h);
}

static void __attribute__((constructor)) rectangle_init() {
    i = calloc(1, sizeof(interface_desc));
    i->draw = rectangle_draw;
    i->move = rectangle_move;
}

static void __attribute__((destructor)) rectangle_free() {
    free(i);
}
