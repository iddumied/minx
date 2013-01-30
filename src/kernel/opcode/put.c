#include "kernel/opcodes.h"

/*
 * Command:                 PUT
 * Parameters:              3, heap-address, register-address, register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_put_func(uint64_t *params) {
    int result;

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
                "into heap %"PRIu64" at offset %"PRIu64" %u bytes from %#010"PRIx64,
                "into heap %"PRIu64" at offset %"PRIu64" %u bytes from %"PRIu64,
                minx_registers_find_register(params[0])->value,
                minx_registers_find_register(params[1])->value,
                (unsigned int)minx_registers_find_register(params[2])->value,
                minx_registers_find_register(params[3])->value
                );
#endif 

    if( minx_registers_find_register(params[2])->value > 8) {
        FATAL_DESC_ERROR("Cannot put more than 8 bytes!");
    }

    result = minx_kernel_heap_put( minx_registers_find_register(params[0])->value, /* the heap */
                                minx_registers_find_register(params[1])->value, /* the offset */
                                (unsigned int)minx_registers_find_register(params[2])->value, /* the size */
                                minx_registers_find_register(params[3])->value); /* the value */

    if(result)
        setbit(minx_registers_find_register(statusregister)->value, PUT_BIT);
    else
        clrbit(minx_registers_find_register(statusregister)->value, PUT_BIT);

}
