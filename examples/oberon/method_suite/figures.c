#include "figures.h"

typedef struct figure_desc {
    interface i;
} figure_desc;

void figure_init(figure f, interface i) {
    f->i = i;
}

void figure_draw(figure f) {
    f->i->draw(f);
}

void figure_move(figure f, int dx, int dy) {
    f->i->move(f, dx, dy);
}
