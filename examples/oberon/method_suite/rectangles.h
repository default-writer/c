#ifndef _RECTANGLES_H_
#define _RECTANGLES_H_

#include "config.h"
#include "figures.h"

typedef struct rectangle* rectangle_pointer;

typedef struct rectangle_type* rectangle_type_pointer;
typedef struct rectangle_data* rectangle_data_pointer;

typedef struct rectangle_type {
    figure_type figure;
    void (*draw)(rectangle_data_pointer f);
    void (*move)(rectangle_data_pointer f, int dx, int dy);
} rectangle_type;

typedef struct rectangle_data {
    figure_data figure;
    int x, y, w, h;
} rectangle_data;

#endif // _RECTANGLES_H_
