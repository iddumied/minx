#include "kernel/opcodes.h"

/**
 * @brief Opcode MUL
 *
 * Command:                 MUL
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_mul_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64") * R%"PRIu64" (%#010"PRIx64")",
            "R%"PRIu64" (%"PRIu64") * R%"PRIu64" (%"PRIu64")",
            params[0], r1->value,
            params[1], r2->value
            );
#endif

    Register *akku = minx_registers_find_register(akku_register_number);
    Register *status = minx_registers_find_register(statusregister);

    if( minx_util_check_multiplication_overflow64(r1->value, r2->value) ) {
        setbit(status->value, OVERFLOW_BIT);
    }

    akku->value = r1->value * r2->value;

    if(akku->value == 0) {
        setbit(status->value, ZERO_BIT);
    }

}
