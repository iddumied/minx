#include "opcodes.h"

/*
 * Command:                 READ
 * Parameters:              3, heap-address, register-address, register-address, register-address
 * Affects Program Pointer: NO
 *
 * Read from heap (addr from reg)
 *      at offset (offs from reg)
 *      n bytes   (num from reg)
 *      into register (register hardcoded)
 *
 */
void opc_read_func(void) {
    int result;

#ifdef DEBUGGING 
    EXPLAIN_OPCODE_WITH("read",
            "from heap %"PRIu64" at offset %"PRIu64" %"PRIu64" Bytes into %"PRIu64,
            minx_registers_find_register(opc_p->p[0])->value,
            minx_registers_find_register(opc_p->p[1])->value,
            minx_registers_find_register(opc_p->p[2])->value,
            opc_p->p[3]
            );
#endif 

    if(minx_registers_find_register(opc_p->p[2])->value > 8) {
        FATAL_DESC_ERROR("Cannot read more than 8 bytes!");
    }

    result = minx_kernel_heap_read(minx_registers_find_register(opc_p->p[0])->value, /* the heap */
                                minx_registers_find_register(opc_p->p[1])->value, /* the offset */
                                minx_registers_find_register(opc_p->p[2])->value, /* the bytecount */
                                &(minx_registers_find_register(opc_p->p[3])->value)); /* the dest */

    if(result)
        setbit(statusregister, READ_BIT);
    else
        clrbit(statusregister, READ_BIT);

}
