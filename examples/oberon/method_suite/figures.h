#ifndef _FIGURES_H_
#define _FIGURES_H_

#include "config.h"

typedef struct figure_desc* figure;
typedef struct interface_desc* interface;

typedef struct interface_desc {
#if defined(INHERITANCE) || defined(EMBEDDED_INHERITANCE)
    interface i; // interface ineritance
#endif
    void (*draw)(figure f);
    void (*move)(figure f, int dx, int dy);
} interface_desc;

typedef struct figure_desc {
    int version; // value inheritance
#if defined(INHERITANCE) || defined(EMBEDDED_INHERITANCE)
    interface i;
#endif
} figure_desc;

#endif // _FIGURES_H_
