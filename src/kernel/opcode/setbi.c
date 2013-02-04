#include "kernel/opcodes.h"

/*
 * Command:                 SETBI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_setbi_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("set bit %"PRIu64" in register %"PRIu64, params[1], params[0]);
#endif

    Register *r1 = minx_registers_find_register(params[0]);

    if( !(params[1] > REGISTER_SIZE * 8) ) {
        setbit(r1->value, params[1]);
    }
}
