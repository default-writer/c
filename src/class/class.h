#ifndef _CLASS_H_
#define _CLASS_H_

#include "std/common.h"

struct object;
struct class;
struct typeinfo;
struct object_typeinfo;

typedef struct object _object;
typedef struct typeinfo _typeinfo;
typedef struct object_typeinfo _object_typeinfo;

typedef _object* object;
typedef _typeinfo* typeinfo;
typedef _object_typeinfo* object_typeinfo;

typedef struct class _class;
typedef const _class* class;

struct object { };

struct typeinfo {
    const size_t size;
    const char* name;
};

struct object_typeinfo {
    const object ptr;
    const typeinfo typeinfo;
};

struct class {
    object_typeinfo (*create)(typeinfo t);
    void (*destroy)(object_typeinfo base);
};

#endif /* _CLASS_H_ */
