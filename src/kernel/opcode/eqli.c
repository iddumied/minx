#include "kernel/opcodes.h"

/*
 * Command:                 EQLI 
 * Parameters:              1: register-address value
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_eqli_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" and %"PRIu64, params[0], params[1]);
#endif 

    Register *r1 = minx_registers_find_register(akku_register_number);
    Register *r2 = minx_registers_find_register(params[0]);

    r1->value = r2->value == params[1];
}

