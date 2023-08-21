#ifndef _RECTANGLES_H_
#define _RECTANGLES_H_

#include "config.h"
#include "figures.h"

typedef struct rectangle_desc* rectangle;

typedef struct rectangle_desc {
#ifdef EMBEDDED_INHERITANCE
    /* changes in figure_desc not allowed */
    /* direct mapping of figure_desc on rectangle_desc */
    int value;
    interface i;
#else
#ifdef INHERITANCE
    /* changes in figure_desc allowed */
    figure_desc i;
#endif
#endif
    int x, y, w, h;
} rectangle_desc;

#endif // _RECTANGLES_H_
