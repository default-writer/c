/* rectangle.c */

#define RECTANGLE_IMPLEMENTATION
#include "rectangle.h"

#include <stdio.h>
#include <string.h>

Rectangle rectangle_new(unsigned width, unsigned height) {
    RectanglePrivate private = {.width = width, .height = height};
    Rectangle rectangle;
    memcpy(&rectangle.private, &private, sizeof(RectanglePrivate));
    return rectangle;
}

void rectangle_print(const Rectangle *rectangle) {
    RectanglePrivate private;
    memcpy(&private, rectangle->private, sizeof(RectanglePrivate));

    printf("width = %u, height = %u\n", private.width, private.height);
}
