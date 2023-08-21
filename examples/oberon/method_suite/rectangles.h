#ifndef _RECTANGLES_H_
#define _RECTANGLES_H_

#include "figures.h"

typedef struct rectangle_desc* rectangle;

typedef struct rectangle_desc {
    // figure_desc base;
    interface i;
    int x, y, w, h;
} rectangle_desc;

#endif // _RECTANGLES_H_
