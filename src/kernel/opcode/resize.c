#include "kernel/opcodes.h"

/*
 * Command:                 RESIZE 
 * Parameters:              1, heap-address, register-address 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as minx_opc_resizei_func() is almost the same
 *
 */
void minx_opc_resize_func(uint64_t *params) {
    int result;

#ifdef DEBUGGING
    EXPLAIN_OPCODE("heap %"PRIu64" to %"PRIu64" Bytes", 
            minx_registers_find_register(params[0])->value, minx_registers_find_register(params[1])->value);
#endif 

    result = minx_kernel_heap_resize(  minx_registers_find_register(params[0])->value, 
                                    minx_registers_find_register(params[1])->value);

    if( result ) 
        setbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
    else 
        clrbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
}