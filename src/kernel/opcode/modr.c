#include "kernel/opcodes.h"

/*
 * Command:                 MODR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_modr_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") %% R%"PRIu64" (%"PRIu64")",
            params[0], r1->value,
            params[1], r2->value
            );
#endif

    Register *status = minx_registers_find_register(statusregister);

    if( r2->value == 0 ) {
        setbit(status->value, DIVZERO_BIT);
    }
    else {
        r1->value = r1->value % r2->value;
    }
}
