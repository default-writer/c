/* queue/list: prev, next, payload */
struct list {
    /* points to previous node */
    struct list* prev;
    /* points to next node */
    struct list* next;
    /* payload */
    void*  payload;
};
