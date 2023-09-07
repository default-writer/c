#include "type.h"
#include "../new.h"

figure_type_pointer figure;

typedef struct figure_data {
    int version;
} *figure_data_pointer, figure_data;

typedef struct figure_object {
    figure_data_pointer data;
    object_pointer object;
} figure_object;

static figure_pointer figure_new() {
    figure_pointer ptr = calloc(1, sizeof(figure_object) + sizeof(figure_data));
    ptr->data = (figure_data_pointer)(ptr + 1);
    return ptr;
}

static figure_pointer figure_create() {
    figure_pointer ptr = figure_new();
    ptr->object = object->create();
    ptr->data->version = 2;
    printf("figure create v%d\n", ptr->data->version);
    return ptr;
}

static void figure_destroy(figure_pointer ptr) {
    object->destroy(ptr->object);
    printf("figure destroy v%d\n", ptr->data->version);
    free(ptr);
}

static void figure_draw(figure_pointer ptr) {
    printf("figure draw v%d\n", ptr->data->version);
}

static void figure_move(figure_pointer ptr, int dx, int dy) {
    printf("figure move v%d\n", ptr->data->version);
}

static void __attribute__((constructor)) figure_type_init() {
    figure = calloc(1, sizeof(figure_type));
    figure->draw = figure_draw;
    figure->move = figure_move;
    figure->create = figure_create;
    figure->destroy = figure_destroy;
}

static void __attribute__((destructor)) figure_type_free() {
    free(figure);
}