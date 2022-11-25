#ifndef _DATA_H_
#define _DATA_H_

/* object: prev, next, payload */
struct list {
    /* points to previous node */
    struct list* prev;
    /* payload */
    void*  payload;
};

#endif // _DATA_H_
