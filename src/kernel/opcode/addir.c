#include "kernel/opcodes.h"

/*
 * Command:                 ADDIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in register
 */
void minx_opc_addir_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" + %"PRIu64, params[0], params[1]);
#endif

    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(params[0]);
    
    if(minx_util_check_addition_overflow64(r1->value, params[1])) {
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value += params[1];

    if(r1->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
}
