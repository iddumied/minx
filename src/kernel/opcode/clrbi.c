#include "kernel/opcodes.h"

/**
 * @brief Opcode CLRBI
 *
 * Command:                 CLRBI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_clrbi_func(uint64_t *params) {
    
    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") &! (0x01<<%"PRIu64")", 
            params[0], r1->value,
            params[1]);
#endif

    if( !(params[1] > REGISTER_SIZE * 8) ) {
        clrbit(r1->value, params[1]);
    }
}
