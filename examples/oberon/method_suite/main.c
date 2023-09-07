#include <stdio.h>
#include <stdlib.h>

#include "rectangle/type.h"

int main(void) {
    rectangle_pointer r = rectangle->create();
    rectangle->draw(r);
    rectangle->move(r, 10, 20);
    rectangle->destroy(r);
    return 0;
}