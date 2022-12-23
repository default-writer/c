#include "std/common.h"

/*

C type system

Any data struct will have last for bytes as pointer to its type methods
and any type will contain method of_type(...) which will check validity of data

struct some_data
{
    const struct some_data_methods* type;
}

struct some_data_methods
{
    int of_type(void* ptr);
}

extern const struct some_data_methods some_data_methods_type;

int of_type(void* ptr)
{
    const struct some_data_methods* type = &some_data_methods_type;
    struct some_data* ptr = (struct some_data*)ptr;
    return ptr->type == type;
}

const struct some_data_methods some_data_methods_type =
{
    .of_type = of_type
};

*/

struct type
{
    // type ptr
    void* ptr;
};

struct some_base_class;
struct some_class;

union some_class_interface
{
    const struct some_base_class* base;
    const struct some_class* class;
    void* ptr;
};

struct some_base_class
{
    LPTR (*get_type)();
};

struct some_class
{
    LPTR (*get_type)();
};

LPTR get_base_type();
LPTR get_class_type();

const struct some_base_class some_base_class_type =
{
    .get_type = get_base_type
};

const struct some_class some_class_type =
{
    .get_type = get_class_type
};

const union some_class_interface some_class_interface_type = {
    .class = &some_class_type
};

LPTR get_base_type()
{
    const struct some_base_class* type = &some_base_class_type;
    return (LPTR)type;
}

LPTR get_class_type()
{
    const struct some_class* type = &some_class_type;
    return (LPTR)type;
}

int main() {
    const union some_class_interface* i = &some_class_interface_type;
    const struct some_class* class = &some_class_type;
    if (i->base->get_type() == (LPTR)class)
    {
        printf("i is of type class at %llx\n", (LPTR)class);
    }
    return 0;
}