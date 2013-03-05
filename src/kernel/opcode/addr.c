#include "kernel/opcodes.h"

/**
 * @brief Opcode ADDR
 *
 * Command:                 ADDR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in register
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_addr_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" + reg %"PRIu64, params[0], params[1]);
#endif

    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

    if(minx_util_check_addition_overflow64(r1->value, r2->value)) {
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value += r2->value;

    if(r1->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
}
