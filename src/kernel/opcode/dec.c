#include "kernel/opcodes.h"

/*
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_dec_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[0]);
#endif 

    if( minx_registers_find_register(params[0])->value == 0x0000 ) {
        setbit(minx_registers_find_register(statusregister)->value, OVERFLOW_BIT);
    }

    minx_registers_find_register(params[0])->value--;
    
}
