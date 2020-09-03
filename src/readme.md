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
    struct list_context* ctx = &rx->ctx;

    // create list
    const struct list_vtable* list = &list_vt;
    void* payload = (void*)0xdeadbeef;

    list->alloc(&ctx->head, payload);
    struct list* head = list->pop(ctx);
    void* head_payload = head->payload;
    // This line of code is potentially a memory leak, if omitted after a popping out element out from stack. The problem is, that our stack is not smart enogth to keep track of all pointers it pops out of the stack. So itself it will not be able to track situation when free method whould probably never be called. We can  create a new stack with proper init/free cycles (figers crossed) which will track of all popped elements from another stack. 
    list->free(&ctx->head, &head);

    // ensure that data being added to list
    RX_REQUIRE(head_payload == payload);
}
```

