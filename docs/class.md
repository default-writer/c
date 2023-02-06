# class

## table of content

- [class](#class)
  - [table of content](#table-of-content)
  - [1. C data structures and types](#1-c-data-structures-and-types)

## 1. C data structures and types

Example of creating custom run-time type information

```c
struct some_data
{
    struct some_data* type;
}

struct some_data
{
    int of_type(void* ptr);
}

extern struct some_data some_data_type;

int of_type(void* ptr)
{
    struct some_data* type = &some_data_type;
    struct some_data* ptr = (struct some_data*)ptr;
    return ptr->type == type;
}

struct some_data some_data_type =
{
    .of_type = of_type
};
```

[^](#class)
