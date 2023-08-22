#ifndef _FIGURES_H_
#define _FIGURES_H_

#include "config.h"

typedef struct figure* figure_pointer;

typedef struct figure_data {
    int version;
} *figure_data_pointer, figure_data;

typedef struct figure_type {
    void (*draw)(figure_data_pointer f);
    void (*move)(figure_data_pointer f, int dx, int dy);
} *figure_type_pointer, figure_type;

#endif // _FIGURES_H_
