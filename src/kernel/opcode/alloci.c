#include "kernel/opcodes.h"

/*
 * Command:                 ALLOCI
 * Parameters:              1, value
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_alloc_func() is almost the same
 *
 */
void opc_alloci_func(void) {

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("alloci", "%"PRIu64" Bytes", opc_p->p[0]);
#endif

    minx_registers_find_register(akku_register_number) = minx_kernel_heap_alloc(opc_p->p[0]);
    
}
