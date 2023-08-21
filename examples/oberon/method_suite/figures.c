#include "figures.h"

typedef struct figure_desc {
    interface i;
} figure_desc;

void figure_init(figure f, interface i) {
    f->i = i;
}
