#include "kernel/opcodes.h"

/*
 * Command:                 GETSIZE
 * Parameters:              1, heap-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_getsize_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("of heap %"PRIu64" into akku", 
                        minx_registers_find_register(params[0])->value);
#endif 

    Register * akku = minx_registers_find_register(akku_register_number);
    Register * heap = minx_registers_find_register(params[0]);
    akku->value = minx_kernel_heap_get_size(heap->value);
}
