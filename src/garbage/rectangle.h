/* rectangle.h */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#define RECTANGLE_PRIVATE                                                      \
    struct {                                                                   \
        unsigned width, height;                                                \
    }

typedef struct {
    char private[sizeof(RECTANGLE_PRIVATE)];
} Rectangle;

#ifdef RECTANGLE_IMPLEMENTATION
typedef RECTANGLE_PRIVATE RectanglePrivate;
#endif

#undef RECTANGLE_PRIVATE

Rectangle rectangle_new(unsigned width, unsigned height);
void rectangle_print(const Rectangle *rectangle);

#endif /* RECTANGLE_H */

