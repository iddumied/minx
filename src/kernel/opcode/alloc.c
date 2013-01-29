#include "kernel/opcodes.h"

/*
 * Command:                 ALLOC 
 * Parameters:              1, register-address
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_alloci_func() is almost the same
 *
 */
static void opc_alloc_func(void) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("%"PRIu64" Bytes", minx_registers_find_register(opc_p->p[0])->value);
#endif

    minx_registers_find_register(akku_register_number)->value = 
        minx_kernel_heap_alloc(minx_registers_find_register(opc_p->p[0])->value);

}

