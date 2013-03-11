#include "kernel/opcodes.h"

/**
 * @brief Opcode DEC
 *
 * Command:                 DEC
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_dec_func(uint64_t *params) {
    
    Register *status = minx_registers_find_register(statusregister);
    Register *r1 = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64")", 
            "R%"PRIu64" (%"PRIu64")", 
            params[0], r1->value);
#endif 

    if(minx_util_check_decrement_overflow64(r1->value)){
        setbit(status->value, OVERFLOW_BIT);
    }

    r1->value--;

    if(r1->value == 0) {
        setbit(status->value, ZERO_BIT);
    }
    
}
