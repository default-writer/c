// #include "A.h"
#include "B.h"
#include "class/class.h"
#include "common/alloc.h"

extern struct_interface_B interface_B_definition;
static const interface_B instance_interface_B = &interface_B_definition;

static void INIT init() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

static void DESTROY destroy() {
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
}

int main(void) {
#ifndef ATTRIBUTE
    init();
#endif
    object_typeinfo ptr = instance_interface_B->initialize();
    instance_interface_B->print(ptr);
    instance_interface_B->finalize(ptr);
#ifndef ATTRIBUTE
    destroy();
#endif
    return 0;
}
