#ifndef _FIGURES_H_
#define _FIGURES_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct figure_desc* figure;
typedef struct interface_desc* interface;
typedef struct base_interface_desc* base_interface;

typedef struct base_interface_desc {
    interface base;
} base_interface_desc;

typedef struct interface_desc {
    // interface i;
    void (*draw)(figure f);
    void (*move)(figure f, int dx, int dy);
} interface_desc;

typedef struct figure_desc {
    interface i;
    // int value;
} figure_desc;

#endif // _FIGURES_H_
