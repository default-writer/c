#ifndef _PRINT_H_
#define _PRINT_H_

struct list_data;

void list_print_head(struct list_data** const current, void* (*data)(struct list_data*));
void list_print(struct list_data** const current, struct list_data* (*next)(struct list_data*));

#endif // _PRINT_H_
