#include "kernel/opcodes.h"

/**
 * @brief Opcode RESIZE 
 *
 * Command:                 RESIZE 
 * Parameters:              1, heap-address, register-address 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as minx_opc_resizei_func() is almost the same
 *
 * @param params Pointer to parameters for this opcode
 *
 */
void minx_opc_resize_func(uint64_t *params) {
    int result;

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("heap %"PRIu64" to %"PRIu64" Bytes", r1->value, r2->value);
#endif 

    result = minx_kernel_heap_resize(r1->value, r2->value);

    if( result ) 
        setbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
    else 
        clrbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
}
