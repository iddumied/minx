#include "kernel/opcodes.h"

/*
 * Command:                 OR
 * Parameters:              2: register-address, register-address
 * Affects Program Pointer: NO
 *
 * Result in minx_registers_find_register(akku_register_number)->value
 */
void minx_opc_or_func(uint64_t *params) {

    Register *r1 = minx_registers_find_register(akku_register_number);
    Register *r2 = minx_registers_find_register(params[0]);
    Register *r3 = minx_registers_find_register(params[1]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" (%"PRIu64") | R%"PRIu64" (%"PRIu64")", 
            params[0], r2->value,
            params[1], r3->value);
#endif 

    r1->value = r2->value | r3->value;
}
