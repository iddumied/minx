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
    EXPLAIN_OPCODE_WITH("resizei", "heap %"PRIu64" to %"PRIu64" Bytes", 
            opc_p->p[0], opc_p->p[1]);
#endif 

    result = minx_kernel_heap_resize(minx_registers_find_register(opc_p->p[0])->value, opc_p->p[1]);

    if( result ) 
        setbit(statusregister, RESIZE_BIT);
    else 
        clrbit(statusregister, RESIZE_BIT);
}
