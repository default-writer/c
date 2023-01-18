# list

## table of content

- [list](#list)
  - [table of content](#table-of-content)
  - [1 list allocation](#1-list-allocation)
  - [1.1 metadata-free allocation](#11-metadata-free-allocation)
  - [1.2 in-place metadata allocation](#12-in-place-metadata-allocation)

## 1 list allocation

allows to organize sequential access to underlying memory (FIFO/LIFO)

[^](#list)

## 1.1 metadata-free allocation

allocation without metadata being written to the data memory pool except for some static data structures or pointers.

[^](#list)

## 1.2 in-place metadata allocation

allocation where sensitive allocation metadata is placed in the same memory pool where allocated data reside. list metadata and data constructs a recurrent data structure. i.e.

[^](#list)

```c
struct list_data {
    /* data */
    void** data;
    /* next element */
    struct list_data* next;
};
```
