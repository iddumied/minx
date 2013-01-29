#include "opcodes.h"

/*
 * Command:                 GETSIZE
 * Parameters:              1, heap-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_getsize_func(void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("GETSIZE", "of heap %"PRIu64" into akku", 
                        minx_registers_find_register(opc_p->p[0])->value);
#endif 

    minx_registers_find_register(akku_register_number)->value = minx_kernel_heap_get_size(find_register(opc_p->p[0])->value);
}
