// #include "A.h"
#include "B.h"
#include "class/class.h"
#include "common/alloc.h"

extern struct_interface_B interface_B_definition;
static const interface_B instance_interface_B = &interface_B_definition;

int main(void) {
    object_typeinfo ptr = instance_interface_B->initialize();
    instance_interface_B->print(ptr);
    instance_interface_B->finalize(ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    global_statistics();
#endif
    return 0;
}
