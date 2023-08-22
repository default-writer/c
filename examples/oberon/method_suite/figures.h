#ifndef _FIGURES_H_
#define _FIGURES_H_

#include "config.h"

typedef struct figure_type* figure_type_pointer;
typedef struct figure_data* figure_data_pointer;

typedef struct figure_type {
    void (*draw)(figure_data_pointer f);
    void (*move)(figure_data_pointer f, int dx, int dy);
} figure_type;

typedef struct figure_data {
    int version;
} figure_data;

#endif // _FIGURES_H_
