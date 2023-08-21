#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "figures.h"
#include "rectangles.h"

extern void rectangle_new(rectangle* r);

int main(void) {
    rectangle r = 0;
    rectangle_new(&r);
    r->x = 3;
    r->y = 4;
    r->w = 11;
    r->h = 20;
    figure f = (figure)r;
    interface i = (interface)f->i;
    if (i && i->draw) {
        i->draw((figure)r);
    }
    if (i && i->move) {
        i->move((figure)r, 10, 20);
    }
    free(r);
    return 0;
}
