#ifndef _CLASS_H_
#define _CLASS_H_

#include "std/common.h"

struct object;
struct class;
struct typeinfo;
struct object_typeinfo;

typedef struct object struct_object;
typedef struct typeinfo struct_typeinfo;
typedef struct object_typeinfo struct_object_typeinfo;
typedef struct class struct_class;

typedef struct_object* object;
typedef struct_typeinfo* typeinfo;
typedef struct_object_typeinfo* object_typeinfo;
typedef struct_class* class;

struct object { };

struct class {
    object_typeinfo (*create)(typeinfo t);
    void (*destroy)(object_typeinfo base);
};

struct typeinfo {
    const size_t size;
    const char* name;
};

struct object_typeinfo {
    const object ptr;
    const typeinfo typeinfo;
};

#endif /* _CLASS_H_ */
