#include "kernel/opcodes.h"

/*
 * Command:                 INC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_inc_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[0]);
#endif 

    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(params[0]);

    if(minx_util_check_increment_overflow64(r1->value)) {
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value++;

    if(r1->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
}
