#include "kernel/opcodes.h"

/*
 * Command:                 ALLOCI
 * Parameters:              1, value
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as minx_opc_alloc_func() is almost the same
 *
 */
void minx_opc_alloci_func(uint64_t *params) {

#ifdef DEBUGGING
    EXPLAIN_OPCODE("%"PRIu64" Bytes", params[0]);
#endif

    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(akku_register_number);
    
    r1->value = minx_kernel_heap_alloc(params[0]);

    if(r1->value == MINX_KERNEL_HEAP_ERROR) {
        setbit(status->value, ALLOC_BIT);
    }
}
