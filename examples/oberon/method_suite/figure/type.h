#ifndef _FIGURES_TYPE_H_
#define _FIGURES_TYPE_H_

#include "data.h"

typedef struct figure_type {
    object_type object;
    void (*draw)(figure_pointer ptr);
    void (*move)(figure_pointer ptr, int dx, int dy);
    figure_pointer (*create)(void);
    void (*destroy)(figure_pointer pointer);
} *figure_type_pointer, figure_type;

extern figure_type_pointer figure;

#endif /* _FIGURES_TYPE_H_ */