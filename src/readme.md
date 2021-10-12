# Class

## Sep 03 

- I will try to create a modification of a list with embedded GC.
- Also, i think i need to restructure code to be able to make a lazy **copy/paste** operations
- Probably, i need to separate list data structure from it's different implementations

### Examples

### Tests 

- test_list_alloc_pop_payload

If we omit call to `list->free(ctx, &head);` we will not free it and this will lead to memory leakage. 

There 2 strategies, *easy* and *lazy*: 

- **Easy way**
    
    Simply check every line of code manually and count all matching pairs of memory allocation funcitons and memory freeing functions. And, if it is matched properly, there should not be non-released to OS memory blocks at all.

- **Lazy way**

    Our case. We are lazy, so we will do something awkward, like create additional layer of abstraction which will do job above for us, while we will watch new episode of Black Mirror on Netflix.

```c++
RX_TEST_CASE(myTestSuite, test_list_alloc_pop_payload, .fixture = test_fixture)
{
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    struct list** ctx = &rx->ctx.head;

    /* create list */
    const struct list_vtable* list = &list_vt;
    void* payload = (void*)0xdeadbeef;

    list->alloc(ctx, payload);
    struct list* head = list->pop(ctx);
    void* head_payload = head->payload;
    list->free(ctx, &head);

    /* ensure that data being added to list */
    RX_REQUIRE(head_payload == payload);
}
```

