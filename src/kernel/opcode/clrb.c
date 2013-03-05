#include "kernel/opcodes.h"

/**
 * @brief Opcode CLRB
 *
 * Command:                 CLRB
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_clrb_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") &! R%"PRIu64" (0x01<<%"PRIu64")", 
            params[0], r1->value,
            params[1], r2->value);
#endif

    if( !(r2->value > REGISTER_SIZE * 8) ) {
        clrbit(r1->value, r2->value);
    }
}
