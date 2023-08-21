typedef struct rectangle_desc* rectangle;

typedef struct rectangle_desc {
    int x, y, w, h;
} rectangle_desc;

void rectangle_new(rectangle* r);
