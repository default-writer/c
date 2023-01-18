#ifndef _PLAYGROUND_LIST_H_
#define _PLAYGROUND_LIST_H_

#include "std/common.h"

struct list {
    void (*init)();
    void (*destroy)();
    void (*push)(void* data);
    void* (*pop)();    
    void* (*peek)();
};

#endif // _PLAYGROUND_LIST_H_