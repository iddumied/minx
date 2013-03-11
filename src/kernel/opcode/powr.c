#include "kernel/opcodes.h"

/**
 * @brief Opcode POWR
 *
 * Command:                 POWR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_powr_func(uint64_t *params) {
    
    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64") ** %"PRIu64" (%#010"PRIx64")",
            "R%"PRIu64" (%"PRIu64") ** %"PRIu64" (%"PRIu64")",
            params[0], r1->value,
            params[1], r2->value
            );
#endif

    Register *status = minx_registers_find_register(statusregister);

    if(minx_util_check_power_overflow64(r1->value, r2->value)) {
        setbit(status->value, OVERFLOW_BIT);
    }

    /* no statuses yet: no overflow-check! */
    r1->value = powl(r1->value, r2->value);

    if( r1->value == 0 ) {
        setbit(status->value, ZERO_BIT);
    }

}
