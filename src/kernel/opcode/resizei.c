#include "kernel/opcodes.h"

/*
 * Command:                 RESIZEI
 * Parameters:              1, heap-address, value 
 * Affects Program Pointer: NO
 *
 * Breaking DRY rule here, as opc_resize_func() is almost the same
 *
 */
void opc_resizei_func(void) {
    int result;
#ifdef DEBUGGING
    EXPLAIN_OPCODE("heap %"PRIu64" to %"PRIu64" Bytes", 
            opc_p->p[0], opc_p->p[1]);
#endif 

    result = minx_kernel_heap_resize(minx_registers_find_register(opc_p->p[0])->value, opc_p->p[1]);

    if( result ) 
        setbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
    else 
        clrbit(minx_registers_find_register(statusregister)->value, RESIZE_BIT);
}
