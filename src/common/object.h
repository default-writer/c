#ifndef _OBJECT_H_
#define _OBJECT_H_

struct list_data;

void list_init(struct list_data** const current);
void list_destroy(struct list_data** const current);

#endif // _OBJECT_H_