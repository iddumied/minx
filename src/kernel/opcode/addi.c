#include "kernel/opcodes.h"

/*
 * Command:                 ADDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
void minx_opc_addi_func(uint64_t *params) {
    
    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(akku_register_number);
    Register *r2 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") + %"PRIu64, 
            params[0], r2->value,
            params[1]);
#endif

    if (minx_util_check_addition_overflow64(r2->value, params[1])) {
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value = r2->value + params[1];

    if(r1->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
}
