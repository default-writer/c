#ifndef _OBJECT_TYPE_H_
#define _OBJECT_TYPE_H_

#include "data.h"

typedef struct object_type {
    object_pointer (*create)(void);
    void (*destroy)(object_pointer ptr);
} *object_type_pointer, object_type;

extern object_type_pointer object;

#endif /* _OBJECT_TYPE_H_ */