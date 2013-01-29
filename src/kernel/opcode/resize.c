#include "kernel/opcodes.h"

/*
 * Command:                 RESIZE 
 * Parameters:              1, heap-address, register-address 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_resizei_func() is almost the same
 *
 */
void opc_resize_func(void) {
    int result;

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH("resize", "heap %"PRIu64" to %"PRIu64" Bytes", 
            minx_registers_find_register(opc_p->p[0])->value, minx_registers_find_register(opc_p->p[1])->value);
#endif 

    result = minx_kernel_heap_resize(  minx_registers_find_register(opc_p->p[0])->value, 
                                    minx_registers_find_register(opc_p->p[1])->value);

    if( result ) 
        setbit(statusregister, RESIZE_BIT);
    else 
        clrbit(statusregister, RESIZE_BIT);
}
