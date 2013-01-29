#include "kernel/opcodes.h"

/*
 * Command:                 FREE 
 * Parameters:              1, heap-address
 * Affects Program Pointer: NO
 *
 * This opcode does not really remove the memory, it just marks it as unused. So
 * if later on memory is required, this one can be used.
 *
 */
void opc_free_func(void) {
    int result;
#ifdef DEBUGGING
    EXPLAIN_OPCODE("heap %"PRIu64, minx_registers_find_register(opc_p->p[0])->value);
#endif

    result = minx_kernel_heap_free(minx_registers_find_register(opc_p->p[0])->value);

    if(result)
        setbit(minx_registers_find_register(statusregister)->value, FREE_BIT);
    else
        clrbit(minx_registers_find_register(statusregister)->value, FREE_BIT);
}
