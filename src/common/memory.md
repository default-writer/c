# 1. memory allocation variants

## 1.1. metadata free allocation

allocation without metadata being written to data memory pool with except of some static data structures or pointers.

## 1.2. mixed allocation

allocation where sensitive allocation metadata is placed in the same memory pool where allocated data resides.

## 1.2.1 forward reference allocation

allocation where allocator calculates the position of metadata token and places the metadata to the end of this region, returning pointer to the start of the region, that is why currently allocated region being written by user program is placed whithin the allocated region.

## 1.2.2 backward reference allocation

the same as above, except that metadata is written first, protecting current metadata block from casual overwrites. it is sligtly better in terms of safety but will also have the same caveeats in case of buffer underflow, allowing corruption of control data.

## 1.2 separate allocation

allocation which completely separates data allocation pools as sensitive allocation metadata for manipulation allocations. this is the safest approach in allocation of data.

it is similar to pp.1.1 except that pp.1.1 is completetely omits metadata semantics with exception of fixed-size generic types or static data structures.
