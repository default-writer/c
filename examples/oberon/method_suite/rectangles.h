typedef struct rectangle_desc* rectangle;
typedef struct figure_desc* figure;

typedef struct rectangle_desc {
    figure f;
    int x, y, w, h;
} rectangle_desc;

void rectangle_new(rectangle* r);

void rectangle_destroy(rectangle r);

void rectangle_draw(figure f);

void rectangle_move(figure f, int dx, int dy);
