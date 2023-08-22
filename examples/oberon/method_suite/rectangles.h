#ifndef _RECTANGLES_H_
#define _RECTANGLES_H_

#include "figures.h"

typedef struct rectangle* rectangle_pointer;

typedef struct rectangle_data {
    figure_data figure;
    int x, y, w, h;
} *rectangle_data_pointer, rectangle_data;

typedef struct rectangle_type {
    figure_type figure;
    void (*draw)(rectangle_data_pointer f);
    void (*move)(rectangle_data_pointer f, int dx, int dy);
} *rectangle_type_pointer, rectangle_type;

#endif // _RECTANGLES_H_
