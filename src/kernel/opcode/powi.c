#include "kernel/opcodes.h"

/*
 * Command:                 POWI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_powi_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") ** %"PRIu64,
            params[0], r1->value,
            params[1]
            );
#endif

    Register *akku = minx_registers_find_register(akku_register_number);
    Register *status = minx_registers_find_register(statusregister);

    /* no statuses yet: no overflow-check! */
    akku->value = powl(r1->value, params[1]);

    if( akku->value == 0 ) {
        setbit(status->value, ZERO_BIT);
    }

}
