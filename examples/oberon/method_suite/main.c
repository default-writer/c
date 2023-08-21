#include <stdio.h>
#include <stdlib.h>

#include "figures.h"
#include "rectangles.h"

extern interface rectangle_i;

int main(void) {
    interface i = rectangle_i;
    rectangle r = 0;
    rectangle_new(&r);
    r->x = 1;
    r->y = 1;
    r->w = 2;
    r->h = 4;
    i->draw((figure)r);
    i->move((figure)r, 10, 20);
    free(r);
    return 0;
}
