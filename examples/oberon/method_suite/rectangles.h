#ifndef _RECTANGLES_H_
#define _RECTANGLES_H_

#include "config.h"
#include "figures.h"

typedef struct rectangle_desc* rectangle;

typedef struct rectangle_desc {
    /* changes in figure_desc allowed */
    figure_desc i;
    int x, y, w, h;
} rectangle_desc;

#endif // _RECTANGLES_H_
