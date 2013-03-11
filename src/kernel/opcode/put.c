#include "kernel/opcodes.h"

/**
 * @brief Opcode PUT
 *
 * Command:                 PUT
 * Parameters:              3, heap-address, register-address, register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_put_func(uint64_t *params) {
    int result;

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);
    Register *r3 = minx_registers_find_register(params[2]);
    Register *r4 = minx_registers_find_register(params[3]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
                "into heap %#010"PRIx64" at offset %010"PRIx64" %u bytes from %#010"PRIx64,
                "into heap %"PRIu64" at offset %"PRIu64" %u bytes from %"PRIu64,
                r1->value,
                r2->value,
                (unsigned int)r3->value,
                r4->value
                );
#endif 

    if( minx_registers_find_register(params[2])->value > 8) {
        FATAL_DESC_ERROR("Cannot put more than 8 bytes!");
    }

    result = minx_kernel_heap_put(  r1->value, /* the heap */
                                    r2->value, /* the offset */
                                    (unsigned int)r3->value, /* the size */
                                    r4->value); /* the value */

    if(result)
        setbit(minx_registers_find_register(statusregister)->value, PUT_BIT);
    else
        clrbit(minx_registers_find_register(statusregister)->value, PUT_BIT);

}
