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

struct typeinfo {
    const size_t size;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* name;
#endif
};

struct class {
    object_typeinfo (*create)(typeinfo ti);
    void (*destroy)(object_typeinfo ptr);
};

struct object_typeinfo {
    const object object;
    const typeinfo typeinfo;
};

#endif /* _CLASS_H_ */
