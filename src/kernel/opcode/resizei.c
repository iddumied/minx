#include "kernel/opcodes.h"

/**
 * @brief Opcode RESIZEI
 *
 * Command:                 RESIZEI
 * Parameters:              1, heap-address, value 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as minx_opc_resize_func() is almost the same
 *
 * @param params Pointer to parameters for this opcode
 *
 */
void minx_opc_resizei_func(uint64_t *params) {
    int result;

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "heap %#010"PRIx64" to %#010"PRIx64" Bytes", 
            "heap %"PRIu64" to %"PRIu64" Bytes", 
            params[0], params[1]);
#endif 

    Register *r1 = minx_registers_find_register(params[0]);
    result = minx_kernel_heap_resize(r1->value, params[1]);

    if( result ) 
        setbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
    else 
        clrbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
}
