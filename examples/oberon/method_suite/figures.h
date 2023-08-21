typedef struct figure_desc* figure;
typedef struct interface_desc* interface;

typedef struct interface_desc {
    void (*draw)(figure f);
    void (*move)(figure f, int dx, int dy);
} interface_desc;
