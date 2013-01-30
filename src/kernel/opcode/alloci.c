#include "kernel/opcodes.h"

/*
 * Command:                 ALLOCI
 * Parameters:              1, value
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_alloc_func() is almost the same
 *
 */
void opc_alloci_func(uint64_t *params) {

#ifdef DEBUGGING
    EXPLAIN_OPCODE("%"PRIu64" Bytes", params[0]);
#endif

    minx_registers_find_register(akku_register_number)->value = 
        minx_kernel_heap_alloc(params[0]);
    
}
