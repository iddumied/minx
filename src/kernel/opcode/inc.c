#include "kernel/opcodes.h"

/*
 * Command:                 INC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void opc_inc_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[0]);
#endif 

    if( minx_registers_find_register(params[0])->value == 0xFFFF ) {
        setbit(minx_registers_find_register(statusregister)->value, OVERFLOW_BIT);
    }

    minx_registers_find_register(params[0])->value++;
}
