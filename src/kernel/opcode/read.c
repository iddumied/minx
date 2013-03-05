#include "kernel/opcodes.h"

/**
 * @brief Opcode READ
 *
 * Command:                 READ
 * Parameters:              3, heap-address, register-address, register-address, register-address
 * Affects Program Pointer: NO
 *
 * Read from heap (addr from reg)
 *      at offset (offs from reg)
 *      n bytes   (num from reg)
 *      into register (register hardcoded)
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_read_func(uint64_t *params) {
    int result;

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);
    Register *r3 = minx_registers_find_register(params[2]);
    Register *r4 = minx_registers_find_register(params[3]);

#ifdef DEBUGGING 
    EXPLAIN_OPCODE(
            "from heap %"PRIu64" at offset %"PRIu64" %"PRIu64" Bytes into %"PRIu64,
            r1->value,
            r2->value,
            r3->value,
            params[3]
            );
#endif 

    if(minx_registers_find_register(params[2])->value > 8) {
        FATAL_DESC_ERROR("Cannot read more than 8 bytes!");
    }

    result = minx_kernel_heap_read( r1->value, /* the heap */
                                    r2->value, /* the offset */
                                    r3->value, /* the bytecount */
                                    &(r4->value)); /* the dest */

    if(result)
        setbit(minx_registers_find_register(statusregister)->value, READ_BIT);
    else
        clrbit(minx_registers_find_register(statusregister)->value, READ_BIT);

}
