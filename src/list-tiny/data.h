/* queue/list: prev, next, payload */
struct list {
    /* points to previous node */
    struct list* prev;
    /* payload */
    void*  payload;
};
