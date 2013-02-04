#include "kernel/opcodes.h"

/*
 * Command:                 SUBI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_subi_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") - %"PRIu64"", 
            params[0], r1->value,
            params[2]
            );
#endif

    Register *akku = minx_registers_find_register(akku_register_number);
    Register *status = minx_registers_find_register(statusregister);

    if(params[1] > r1->value) {
        setbit(status->value, OVERFLOW_BIT);
    }

    akku->value = r1->value - params[1];

    if(akku->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
}
