typedef struct figure_desc* figure;
typedef struct interface_desc* interface;

typedef struct interface_desc {
    void (*draw)(figure f);
    void (*move)(figure f, int dx, int dy);
} interface_desc;

void figure_init(figure f, interface i);

void figure_draw(figure f);

void figure_move(figure f, int dx, int dy);
