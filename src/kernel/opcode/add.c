#include "kernel/opcodes.h"

/**
 * @brief Opcode ADD
 *
 * Command:                 ADD
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in akku
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_add_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" + reg %"PRIu64, params[0], params[1]);
#endif

    Register *r1 = minx_registers_find_register(akku_register_number);
    Register *r2 = minx_registers_find_register(params[0]);
    Register *r3 = minx_registers_find_register(params[1]);
    Register *status    = minx_registers_find_register(statusregister);

    /*
     * Do some checks if there will be a overflow and set the overflow bit if.
     *
     * if uint64_max - r2->value is less than r3->value, there will be a 
     * overflow
     */
    if (minx_util_check_addition_overflow64(r2->value, r3->value)) {
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value = r2->value + r3->value;

    /* test if result is zero, set zero bit if so */
    if( r1->value == 0 ) {
        setbit(status->value, ZERO_BIT);
    }
}
