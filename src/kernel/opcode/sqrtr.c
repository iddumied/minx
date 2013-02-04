#include "kernel/opcodes.h"

/*
 * Command:                 SQRTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_sqrtr_func(uint64_t *params) {
    
    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64")", params[0], r1->value);
#endif

    Register *status = minx_registers_find_register(statusregister);

    /* no status yet: no check for overflow */
    r1->value = sqrtl(r1->value);

    if( r1->value == 0 ) {
        setbit(status->value, ZERO_BIT);
    }
}
