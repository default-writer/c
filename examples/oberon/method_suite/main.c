#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "figures.h"
#include "rectangles.h"

extern rectangle_pointer rectangle_new();
extern void rectangle_delete(rectangle_pointer r);
extern void rectangle_draw(rectangle_pointer r);
extern void rectangle_move(rectangle_pointer r, int dx, int dy);

int main(void) {
    rectangle_pointer r = rectangle_new();
    rectangle_draw(r);
    rectangle_move(r, 10, 20);
    rectangle_delete(r);
    return 0;
}
