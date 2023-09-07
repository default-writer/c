#ifndef _RECTANGLES_TYPE_H_
#define _RECTANGLES_TYPE_H_

#include "data.h"

typedef struct rectangle_type {
    figure_type figure;
    void (*draw)(rectangle_pointer ptr);
    void (*move)(rectangle_pointer ptr, int dx, int dy);
    rectangle_pointer (*create)(void);
    void (*destroy)(rectangle_pointer pointer);
} *rectangle_type_pointer, rectangle_type;

extern rectangle_type_pointer rectangle;

#endif /* _RECTANGLES_TYPE_H_ */