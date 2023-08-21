typedef struct figure_desc* figure;
typedef struct rectangle_desc* rectangle;
typedef struct interface_desc* interface;

typedef struct rectangle_desc {
    interface i;
    int x, y, w, h;
} rectangle_desc;
