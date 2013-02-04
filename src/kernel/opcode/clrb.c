#include "kernel/opcodes.h"

/*
 * Command:                 CLRB
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_clrb_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("clear bit %"PRIu64" in register %"PRIu64, r2->value, params[0]);
#endif

    if( !(r2->value > REGISTER_SIZE * 8) ) {
        clrbit(r1->value, r2->value);
    }
}
