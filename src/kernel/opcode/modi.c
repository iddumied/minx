#include "kernel/opcodes.h"

/**
 * @brief Opcode MODI
 *
 * Command:                 MODI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_modi_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") %% %"PRIu64,
            params[0], r1->value,
            params[1]
            );
#endif

    Register *akku = minx_registers_find_register(akku_register_number);
    Register *status = minx_registers_find_register(statusregister);

    if( params[1] == 0 ) {
        setbit(status->value, DIVZERO_BIT);
    }
    else {
        akku->value = r1->value % params[1];
    }
}