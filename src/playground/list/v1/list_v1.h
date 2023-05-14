#ifndef _LIST_V1_H_
#define _LIST_V1_H_

#include "std/common.h"

struct list {
    void (*init)(void);
    void (*destroy)(void);
    void (*push)(void* data);
    void* (*pop)(void);
    void* (*peek)(void);
};

#endif /* _LIST_V1_H_ */
