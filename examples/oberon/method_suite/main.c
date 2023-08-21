#include <stdio.h>
#include <stdlib.h>

#include "figures.h"
#include "rectangles.h"

extern void rectangle_new(rectangle* r);

int main(void) {
    rectangle r = 0;
    rectangle_new(&r);
    // printf("%016x\n", r->base.value);
    r->x = 1;
    r->y = 1;
    r->w = 2;
    r->h = 4;
    // r->base.i->draw((figure)r);
    // r->base.i->move((figure)r, 10, 20);
    r->i->draw((figure)r);
    r->i->move((figure)r, 10, 20);
    free(r);
    return 0;
}
