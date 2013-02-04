#include "kernel/opcodes.h"

/*
 * Command:                 ALLOC 
 * Parameters:              1, register-address
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as minx_opc_alloci_func() is almost the same
 *
 */
void minx_opc_alloc_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("%"PRIu64" Bytes", minx_registers_find_register(params[0])->value);
#endif

    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(akku_register_number);

    Register *r2 = minx_registers_find_register(params[0]);

    r1->value = minx_kernel_heap_alloc(r2->value);

    if(r1->value == MINX_KERNEL_HEAP_ERROR) {
        setbit(status->value, ALLOC_BIT);
    }
}

