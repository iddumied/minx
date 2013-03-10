#include "kernel/opcodes.h"

/**
 * @brief Opcode SUBIR
 *
 * Command:                 SUBIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_subir_func(uint64_t *params) {
    
    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") -= %"PRIu64, 
            params[0], r1->value,
            params[1]
            );
#endif

    Register *status = minx_registers_find_register(statusregister);

    if(minx_util_check_subtraction_overflow64(params[1], r1->value)) {
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value = r1->value - params[1];

    if(r1->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
}
