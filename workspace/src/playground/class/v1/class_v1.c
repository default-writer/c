/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "playground/class/v1/class_v1.h"
#include "common/memory.h"
#include "list-micro/data.h"

/* class data definition */
extern struct class_data class_data;

/* class data */
struct class_data {
    /* data pointer */
    void* ptr;
    /* list */
};

/* implementation */

/* private */

/* class data list */
static struct list_data list_data;
/* class data list reference */
static struct list_data* class_list_data = &list_data;

/* definition */
extern const struct list list_micro_definition;

/* definition */
static const struct list* list = &list_micro_definition;

/* creates the class instance */
static struct class_data* _new(void);
/* deletes the class instance */
static void _delete(struct class_data* ptr);

/* proxy for the class function get_data() */
static void* class_get(void);
/* proxy for the class function set_data( void*)*/
static void class_set(void* data);

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct class_data);

/* allocates memory pointer */
static struct class_data* _new(void) {
    /* returns class object */
    return memory->alloc(_size);
}

/* releases memory pointer */
static void _delete(struct class_data* class) {
    /* releases the pointer */
    memory->free(class, _size);
}

/* initializes the new context's head element */
static struct class_data* class_new(void) {
    /* creates emtpy data chunk */
    return _new();
}

/* destroys the memory stack */
static void class_delete(struct class_data* class) {
    /* releases the pointer */
    _delete(class);
}

static void* class_get_data(struct class_data* class) {
    return class->ptr;
}

static void class_set_data(struct class_data* class, void* data) {
    class->ptr = data;
}

static void class_push(struct class_data* class) {
    /* pushes to the list */
    list->push(&class_list_data, class);
}

static struct class_data* class_pop(void) {
    /* pops from the list */
    return list->pop(&class_list_data);
}

static void* class_get(void) {
    /* returns data */
    return class_get_data(list->peek(&class_list_data));
}

static void class_set(void* data) {
    /* updates the data */
    class_set_data(list->peek(&class_list_data), data);
}

/* public */

const struct class class_definition_v1 = {
    .new = class_new,
    .delete = class_delete,
    /* generic methods */
    .push = class_push,
    .pop = class_pop,
    .get = class_get,
    .set = class_set
};
